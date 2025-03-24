#include "Duplo.h"
#include "IExporter.h"
#include "Options.h"
#include "ProcessResult.h"
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

typedef std::tuple<unsigned, std::string> FileLength;
typedef const std::string* StringPtr;
typedef std::unordered_map<unsigned long, std::vector<StringPtr>> HashToFiles;

static constexpr std::size_t TOP_N_LONGEST = 10;

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

    std::tuple<std::vector<SourceFile>, std::vector<bool>, unsigned, unsigned> LoadSourceFiles(
        const std::vector<std::string>& lines,
        unsigned minChars,
        bool ignorePrepStuff,
        IExporterPtr exporter) {

        std::vector<SourceFile> sourceFiles;
        std::vector<bool> matrix;
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

        if (maxLinesPerFile * maxLinesPerFile > matrix.max_size()) {
            std::ostringstream stream;
            stream
                << "Some files have too many lines. You can have files with approximately "
                << std::sqrt(matrix.max_size())
                << " lines at most." << std::endl
                << "Longest files:" << std::endl;
            printLongestFiles(stream, sourceFiles, TOP_N_LONGEST);
            throw std::runtime_error(stream.str().c_str());
        }

        exporter->Log(std::format("{} done.\n\n", lines.size()));

        // Generate matrix large enough for all files
        try {
            matrix.resize(maxLinesPerFile * maxLinesPerFile);
        }
        catch (const std::bad_alloc& ex) {
            std::ostringstream stream;
            stream
                << ex.what() << std::endl
                << "Longest files:" << std::endl;
            printLongestFiles(stream, sourceFiles, TOP_N_LONGEST);
            throw std::runtime_error(stream.str().c_str());
        }

        return std::tuple(std::move(sourceFiles), matrix, files, locsTotal);
    }

    ProcessResult Process(
        const SourceFile& source1,
        const SourceFile& source2,
        std::vector<bool>& matrix,
        const Options& options,
        IExporterPtr exporter) {
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

        unsigned blocks = 0;
        unsigned duplicateLines = 0;

        // make curried function for invoking ReportSeq
        auto reportSeq = [&source1, &source2, &exporter](int line1, int line2, int count) {
            exporter->ReportSeq(
                line1,
                line2,
                count,
                source1,
                source2);
        };

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
                            reportSeq(line1, line2, seqLen);
                            duplicateLines += seqLen;
                            blocks++;
                        }
                    }

                    seqLen = 0;
                }
            }

            if (seqLen >= lMinBlockSize) {
                int line1 = m - seqLen;
                int line2 = n - seqLen;
                if (line1 != line2 || source1 != source2) {
                    reportSeq(line1, line2, seqLen);
                    duplicateLines += seqLen;
                    blocks++;
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
                            reportSeq(y - seqLen, x + y - seqLen, seqLen);
                            duplicateLines += seqLen;
                            blocks++;
                        }
                        seqLen = 0;
                    }
                }

                if (seqLen >= lMinBlockSize) {
                    reportSeq(m - seqLen, n - seqLen, seqLen);
                    duplicateLines += seqLen;
                    blocks++;
                }
            }
        }

        return ProcessResult(blocks, duplicateLines);
    }
}

int Duplo::Run(const Options& options) {

    IExporterPtr exporter = IExporter::CreateExporter(options);
    exporter->Log("Loading and hashing files ... ");

    exporter->WriteHeader();

    auto lines = FileSystem::LoadFileList(options.GetListFilename());
    auto [sourceFiles, matrix, files, locsTotal] = LoadSourceFiles(
        lines,
        options.GetMinChars(),
        options.GetIgnorePrepStuff(),
        exporter);
    auto numFilesToCheck = options.GetFilesToCheck() > 0 ? std::min(options.GetFilesToCheck(), sourceFiles.size()) : sourceFiles.size();

    // hash maps
    HashToFiles hashToFiles;
    for (const auto& s : sourceFiles) {
        for (size_t i = 0; i < s.GetNumOfLines(); i++) {
            hashToFiles[s.GetLine(i).GetHash()].push_back(&s.GetFilename());
        }
    }

    // Compare each file with each other
    ProcessResult processResultTotal;
    for (unsigned i = 0; i < numFilesToCheck; i++) {
        const auto& left = sourceFiles[i];

        // get matching files
        std::unordered_set<StringPtr> matchingFiles;
        for (std::size_t k = 0; k < left.GetNumOfLines(); k++) {
            auto hash = left.GetLine(k).GetHash();
            const auto& filenames = hashToFiles[hash];
            matchingFiles.reserve(filenames.size());
            for (auto& x : filenames) {
                matchingFiles.insert(x);
            }
        }

        ProcessResult processResult =
            Process(
                left,
                left,
                matrix,
                options,
                exporter);

        // files to compare are those that have matching lines
        for (unsigned j = i + 1; j < sourceFiles.size(); j++) {
            const auto& right = sourceFiles[j];
            if ((!options.GetIgnoreSameFilename() || !StringUtil::IsSameFilename(left, right)) && matchingFiles.find(&right.GetFilename()) != matchingFiles.end()) {
                processResult
                    << Process(
                           left,
                           right,
                           matrix,
                           options,
                           exporter);
            }
        }

        if (processResult.Blocks() > 0) {
            exporter->Log(std::format("{} found: {} block(s)\n", left.GetFilename(), processResult.Blocks()));
        } else {
            exporter->Log(std::format("{} nothing found.\n", left.GetFilename()));
        }

        processResultTotal << processResult;
    }

    exporter->WriteFooter(options, files, locsTotal, processResultTotal);

    return processResultTotal.Blocks() > 0
               ? EXIT_FAILURE
               : EXIT_SUCCESS;
}
