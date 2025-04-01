#include "Duplo.h"
#include "IExporter.h"
#include "Options.h"
#include "Block.h"
#include "SourceFile.h"
#include "SourceLine.h"
#include "Utils.h"

#include <algorithm>
#include <cstring>
#include <ctime>
#include <cmath>
#include <format>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <thread>
#include <ranges>

#include <BS_thread_pool.hpp>

typedef std::tuple<unsigned, std::string> FileLength;
typedef const std::string* StringPtr;
typedef std::unordered_map<unsigned long, std::vector<StringPtr>> HashToFiles;
using thread_pool = BS::thread_pool<>;

static constexpr std::size_t TOP_N_LONGEST = 10;

struct ThreadContext {
    std::vector<bool> matrix;
    std::vector<Block> dup_blocks;
    std::size_t num_dup_lines;
};

namespace {
    void printLongestFiles(std::ostringstream &oss, std::vector<SourceFile> &sourceFiles, std::size_t top_n) {
        std::sort(std::begin(sourceFiles),
                  std::end(sourceFiles),
                  [](SourceFile const& l, SourceFile const& r) {
            return l.GetNumOfLines() > r.GetNumOfLines();
        });
        std::for_each_n(std::begin(sourceFiles), top_n, [&oss](SourceFile const& file) {
            oss << file.GetNumOfLines() << ": " << file.GetFilename() << '\n';
        });
    }

    std::tuple<std::vector<SourceFile>, unsigned, unsigned, std::size_t> LoadSourceFiles(
        const std::vector<std::string>& lines,
        unsigned minChars,
        bool ignorePrepStuff,
        unsigned numThreads,
        IExporterPtr exporter) {

        std::vector<SourceFile> sourceFiles;
        size_t maxLinesPerFile = 0;
        int files = 0;
        unsigned long locsTotal = 0;

        // Create vector with all source files
        for (size_t i = 0; i < lines.size(); i++) {
            if (lines[i].size() > 5) {
                SourceFile sourceFile(lines[i], minChars, ignorePrepStuff);
                size_t numLines = sourceFile.GetNumOfLines();
                if (numLines > 0) {
                    files++;
                    sourceFiles.push_back(std::move(sourceFile));
                    locsTotal += numLines;
                    if (maxLinesPerFile < numLines) {
                        maxLinesPerFile = numLines;
                    }
                }
            }
        }

        auto const max_size = std::vector<bool>().max_size();
        if (numThreads * maxLinesPerFile * maxLinesPerFile > max_size) {
            std::ostringstream stream;
            stream
                << "Some files have too many lines, or you're using too many threads.\n"
                << "Using " << numThreads << " thread(s), you can have files with approximately "
                << std::sqrt((double)max_size / numThreads) << " lines at most.\n"
                << "Longest files:\n";
            printLongestFiles(stream, sourceFiles, TOP_N_LONGEST);
            throw std::runtime_error(stream.str().c_str());
        }

        exporter->LogMessage(std::format("{} done.\n\n", lines.size()));

        return std::tuple(std::move(sourceFiles), files, locsTotal, maxLinesPerFile);
    }

    void Process(
        const SourceFile& source1,
        const SourceFile& source2,
        const Options& options,
        ThreadContext &context) {

        // unwrap the context
        auto &[matrix, dup_blocks, num_dup_lines] = context;

        size_t m = source1.GetNumOfLines();
        size_t n = source2.GetNumOfLines();

        // Reset matrix data
        std::fill(std::begin(matrix), std::begin(matrix) + m * n, false);

        // Compute matrix
        for (size_t y = 0; y < m; y++) {
            auto& line = source1.GetLine(y);
            for (size_t x = 0; x < n; x++) {
                if (line == source2.GetLine(x)) {
                    matrix[x + n * y] = true;
                }
            }
        }

        // support reporting filtering by both:
        // - "lines of code duplicated", &
        // - "percentage of file duplicated"
        unsigned lMinBlockSize = std::max(
            (size_t)options.GetMinBlockSize(),
            std::min(
                (size_t)options.GetMinBlockSize(),
                (std::max(n, m) * 100) / options.GetBlockPercentThreshold()));

        // Scan vertical part
        for (size_t y = 0; y < m; y++) {
            unsigned seqLen = 0;
            size_t maxX = std::min(n, m - y);
            for (size_t x = 0; x < maxX; x++) {
                if (matrix[x + n * (y + x)]) {
                    seqLen++;
                } else {
                    if (seqLen >= lMinBlockSize) {
                        int line1 = y + x - seqLen;
                        int line2 = x - seqLen;
                        if (line1 != line2 || source1 != source2) {
                            dup_blocks.emplace_back(&source1, &source2, line1, line2, seqLen);
                            num_dup_lines += seqLen;
                        }
                    }

                    seqLen = 0;
                }
            }

            if (seqLen >= lMinBlockSize) {
                int line1 = m - seqLen;
                int line2 = n - seqLen;
                if (line1 != line2 || source1 != source2) {
                    dup_blocks.emplace_back(&source1, &source2, line1, line2, seqLen);
                    num_dup_lines += seqLen;
                }
            }
        }

        if (source1 != source2) {
            // Scan horizontal part
            for (size_t x = 1; x < n; x++) {
                unsigned seqLen = 0;
                size_t maxY = std::min(m, n - x);
                for (size_t y = 0; y < maxY; y++) {
                    if (matrix[x + y + n * y]) {
                        seqLen++;
                    } else {
                        if (seqLen >= lMinBlockSize) {
                            dup_blocks.emplace_back(&source1, &source2, y - seqLen, x + y - seqLen, seqLen);
                            num_dup_lines += seqLen;
                        }
                        seqLen = 0;
                    }
                }

                if (seqLen >= lMinBlockSize) {
                    dup_blocks.emplace_back(&source1, &source2, m - seqLen, n - seqLen, seqLen);
                    num_dup_lines += seqLen;
                }
            }
        }
    }

    void ProcessRange(
        std::vector<SourceFile>::iterator l_it,
        std::vector<SourceFile>::iterator end_it,
        HashToFiles const& hashToFiles,
        Options const& options,
        IExporterPtr exporter,
        std::mutex &exporter_mtx,
        std::unordered_map<std::thread::id, ThreadContext>& contexts) {

        // get matching files
        std::unordered_set<StringPtr> matchingFiles;
        for (std::size_t k = 0; k < l_it->GetNumOfLines(); k++) {
            auto hash = l_it->GetLine(k).GetHash();
            const auto& filenames = hashToFiles.find(hash)->second;
            matchingFiles.insert(filenames.begin(), filenames.end());
        }

        auto& context = contexts.find(std::this_thread::get_id())->second;

        auto const old_num_blocks = context.dup_blocks.size();

        // compare the file with itself
        Process(*l_it, *l_it, options, context);

        // files to compare with are those that have matching lines
        for (auto r_it = std::next(l_it); r_it != end_it; ++r_it) {
            if (options.GetIgnoreSameFilename() && StringUtil::IsSameFilename(*l_it, *r_it)) {
                continue;
            }
            if (matchingFiles.find(&r_it->GetFilename()) == matchingFiles.end()) {
                continue;
            }
            // compare the file with another file
            Process(*l_it, *r_it, options, context);
        }

        auto const num_blocks_added = context.dup_blocks.size() - old_num_blocks;

        {
            std::scoped_lock sl(exporter_mtx);
            if (num_blocks_added > 0) {
                exporter->LogMessage(std::format("{} found: {} block(s)\n", l_it->GetFilename(), num_blocks_added));
            } else {
                exporter->LogMessage(std::format("{} nothing found.\n", l_it->GetFilename()));
            }
        }
    }
}

int Duplo::Run(const Options& options) {

    IExporterPtr exporter = IExporter::CreateExporter(options);
    std::mutex exporter_mtx;
    exporter->LogMessage("Loading and hashing files ... ");

    exporter->WriteHeader();

    auto lines = FileSystem::LoadFileList(options.GetListFilename());
    auto [sourceFiles, files, locsTotal, max_lines] = LoadSourceFiles(
        lines,
        options.GetMinChars(),
        options.GetIgnorePrepStuff(),
        options.GetNumThreads(),
        exporter);

    auto end_it = sourceFiles.end();
    if (options.GetFilesToCheck() > 0 && options.GetFilesToCheck() < sourceFiles.size()) {
        end_it = std::next(sourceFiles.begin(), options.GetFilesToCheck());
    }

    // hash maps
    HashToFiles hashToFiles;
    for (const auto& s : sourceFiles) {
        for (size_t i = 0; i < s.GetNumOfLines(); i++) {
            hashToFiles[s.GetLine(i).GetHash()].push_back(&s.GetFilename());
        }
    }

    thread_pool pool(options.GetNumThreads());
    std::unordered_map<std::thread::id, ThreadContext> contexts;
    // Generate matrix large enough for all files
    try {
        for (auto const &thread_id : pool.get_thread_ids()) {
            contexts[thread_id] = {std::vector<bool>(max_lines * max_lines), {}, {}};
        }
    }
    catch (const std::bad_alloc& ex) {
        std::ostringstream stream;
        stream << ex.what() << '\n'
               << "Try reducing the number of threads or exclude some the following longest files:\n"
               << "Longest files:\n";
        printLongestFiles(stream, sourceFiles, TOP_N_LONGEST);
        throw std::runtime_error(stream.str().c_str());
    }

    // add a task in the threadpool to compare each file with all files after it
    for (auto l_it = sourceFiles.begin(), l_end = end_it; l_it != l_end; ++l_it) {
        pool.detach_task([l_it, end_it, &hashToFiles, &options, &exporter, &exporter_mtx, &contexts]{
            ProcessRange(l_it, end_it, hashToFiles, options, exporter, exporter_mtx, contexts);
        });
    }
    pool.wait();

    std::size_t tot_num_dup_blocks = 0;
    std::size_t tot_num_dup_lines = 0;
    for (auto const &[tid, context] : contexts) {
        tot_num_dup_blocks += context.dup_blocks.size();
        tot_num_dup_lines += context.num_dup_lines;
    }

    std::vector<Block> tot_dup_blocks;
    tot_dup_blocks.reserve(tot_num_dup_blocks);
    for (auto const &[tid, context] : contexts) {
        tot_dup_blocks.insert(tot_dup_blocks.end(), context.dup_blocks.begin(), context.dup_blocks.end());
    }

    std::ranges::for_each(tot_dup_blocks, [&exporter](Block const& block) {
        exporter->ReportSeq(block.m_line1, block.m_line2, block.m_count, *block.m_source1, *block.m_source2);
    });

    exporter->WriteFooter(options, files, locsTotal, tot_dup_blocks, tot_num_dup_lines);

    return tot_dup_blocks.empty() ? EXIT_SUCCESS : EXIT_FAILURE;
}
