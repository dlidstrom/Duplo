#include "Duplo.h"
#include "ArgumentParser.h"
#include "HashUtil.h"
#include "Options.h"
#include "SourceFile.h"
#include "SourceLine.h"
#include "Utils.h"
#include "TextFile.h"

#include <nlohmann/json.hpp>
#include "ProcessResult.h"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <ctime>
#include <fstream>
#include <optional>
#include <sstream>
#include <tuple>
#include <unordered_map>
#include <unordered_set>

typedef std::tuple<unsigned, std::string> FileLength;
typedef const std::string* StringPtr;
typedef std::unordered_map<unsigned long, std::vector<StringPtr>> HashToFiles;
using json = nlohmann::json;

class ProcessResult {
    unsigned m_blocks;
    unsigned m_duplicateLines;

public:
    ProcessResult()
        : m_blocks(0),
          m_duplicateLines(0) {
    }

    ProcessResult(unsigned blocks, unsigned duplicateLines)
        : m_blocks(blocks),
          m_duplicateLines(duplicateLines) {
    }

    unsigned Blocks() const {
        return m_blocks;
    }

    unsigned DuplicateLines() const {
        return m_duplicateLines;
    }

    friend ProcessResult operator<<(ProcessResult& left, const ProcessResult& right);
};

ProcessResult operator<<(ProcessResult& left, const ProcessResult& right) {
    left.m_blocks += right.m_blocks;
    left.m_duplicateLines += right.m_duplicateLines;
    return left;
}

namespace {

    std::tuple<std::vector<SourceFile>, std::vector<bool>, unsigned, unsigned> LoadSourceFiles(
        const std::vector<std::string>& lines,
        unsigned minChars,
        bool ignorePrepStuff,
        std::ostream& log) {

        std::vector<SourceFile> sourceFiles;
        std::vector<bool> matrix;
        size_t maxLinesPerFile = 0;
        int files = 0;
        unsigned long locsTotal = 0;
        std::vector<FileLength> longestFiles;
        auto addSorted = [&longestFiles](int numLines, const std::string& filename) {
            longestFiles.emplace_back(numLines, filename);
            std::sort(
                std::begin(longestFiles),
                std::end(longestFiles),
                [](auto l, auto r) { return std::get<0>(l) > std::get<0>(r); });
            if (longestFiles.size() > 10)
                longestFiles.resize(10);
        };

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

                    // keep 10 worst case files
                    if (longestFiles.size() < 10) {
                        addSorted(numLines, lines[i]);
                    } else {
                        auto& [l, r] = longestFiles.back();
                        (void)r;
                        if (l < numLines) {
                            addSorted(numLines, lines[i]);
                        }
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
            for (auto& [l, f] : longestFiles) {
                stream << l << ": " << f << std::endl;
            }

            throw std::runtime_error(stream.str().c_str());
        }

        log
            << lines.size()
            << " done.\n\n";
        // Generate matrix large enough for all files
        try {
            matrix.resize(maxLinesPerFile * maxLinesPerFile);
        }
        catch (const std::bad_alloc& ex) {
            std::ostringstream stream;
            stream
                << ex.what() << std::endl
                << "Longest files:" << std::endl;
            for (auto& [l, f] : longestFiles) {
                stream << l << ": " << f << std::endl;
            }

            throw std::runtime_error(stream.str().c_str());
        }

        return std::tuple(std::move(sourceFiles), matrix, files, locsTotal);
    }

    void ReportSeqJSON(
        int begin,
        int end,
        int src_begin1,
        int src_end1,
        int src_begin2,
        int src_end2,
        const SourceFile& source1,
        const SourceFile& source2,
        json& json_out) {
        json_out.emplace_back(json{
            { "LineCount", end - begin },
            { "SourceFile1", source1.GetFilename() },
            { "StartLineNumber1", src_begin1 },
            { "EndLineNumber1", src_end1 },
            { "SourceFile2", source2.GetFilename() },
            { "StartLineNumber2", src_begin2 },
            { "EndLineNumber2", src_end2 },
            { "Lines", source1.GetLines(begin, end) } });
    }

    void ReportSeq(
        int line1,
        int line2,
        int count,
        bool xml,
        const SourceFile& source1,
        const SourceFile& source2,
        std::ostream& out,
        std::optional<json>& json_out) {
        if (xml) {
            out
                << "    <set LineCount=\"" << count << "\">"
                << std::endl;
            int startLineNumber1 = source1.GetLine(line1).GetLineNumber();
            int endLineNumber1 = source1.GetLine(line1 + count).GetLineNumber();
            out
                << "        <block SourceFile=\"" << source1.GetFilename()
                << "\" StartLineNumber=\"" << startLineNumber1
                << "\" EndLineNumber=\"" << endLineNumber1 << "\"/>"
                << std::endl;
            int startLineNumber2 = source2.GetLine(line2).GetLineNumber();
            int endLineNumber2 = source2.GetLine(line2 + count).GetLineNumber();
            out
                << "        <block SourceFile=\"" << source2.GetFilename()
                << "\" StartLineNumber=\"" << startLineNumber2
                << "\" EndLineNumber=\"" << endLineNumber2 << "\"/>"
                << std::endl;
            out
                << "        <lines xml:space=\"preserve\">"
                << std::endl;
            for (int j = 0; j < count; j++) {
                // replace various characters/ strings so that it doesn't upset the XML parser
                std::string tmpstr = source1.GetLine(j + line1).GetLine();

                // " --> '
                StringUtil::StrSub(tmpstr, "\'", "\"", -1);

                // & --> &amp;
                StringUtil::StrSub(tmpstr, "&amp;", "&", -1);

                // < --> &lt;
                StringUtil::StrSub(tmpstr, "&lt;", "<", -1);

                // > --> &gt;
                StringUtil::StrSub(tmpstr, "&gt;", ">", -1);

                out << "            <line Text=\"" << tmpstr << "\"/>" << std::endl;
            }

            out << "        </lines>" << std::endl;
            out << "    </set>" << std::endl;
        } else if (json_out) {
            ReportSeqJSON(
                    line1,
                    line1 + count,
                    source1.GetLine(line1).GetLineNumber(),
                    source1.GetLine(line1 + count).GetLineNumber(),
                    source2.GetLine(line2).GetLineNumber(),
                    source2.GetLine(line2 + count).GetLineNumber(),
                    source1,
                    source2,
                    json_out.value());
        } else {
            out
                << source1.GetFilename()
                << "(" << source1.GetLine(line1).GetLineNumber() << ")"
                << std::endl;
            out
                << source2.GetFilename()
                << "(" << source2.GetLine(line2).GetLineNumber() << ")"
                << std::endl;
            for (int j = 0; j < count; j++) {
                out << source1.GetLine(j + line1).GetLine() << std::endl;
            }

            out << std::endl;
        }
    }

    ProcessResult Process(
        const SourceFile& source1,
        const SourceFile& source2,
        std::vector<bool>& matrix,
        const Options& options,
        std::ostream& outFile,
        std::optional<json>& json_out) {
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
                            ReportSeq(
                                line1,
                                line2,
                                seqLen,
                                options.GetOutputXml(),
                                source1,
                                source2,
                                outFile,
                                json_out);
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
                    ReportSeq(
                        line1,
                        line2,
                        seqLen,
                        options.GetOutputXml(),
                        source1,
                        source2,
                        outFile,
                        json_out);
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
                            ReportSeq(
                                y - seqLen,
                                x + y - seqLen,
                                seqLen,
                                options.GetOutputXml(),
                                source1,
                                source2,
                                outFile,
                                json_out);
                            duplicateLines += seqLen;
                            blocks++;
                        }
                        seqLen = 0;
                    }
                }

                if (seqLen >= lMinBlockSize) {
                    ReportSeq(
                        m - seqLen,
                        n - seqLen,
                        seqLen,
                        options.GetOutputXml(),
                        source1,
                        source2,
                        outFile,
                        json_out);
                    duplicateLines += seqLen;
                    blocks++;
                }
            }
        }

        return ProcessResult(blocks, duplicateLines);
    }
}

int Duplo::Run(const Options& options) {
    std::streambuf* buf;
    std::streambuf* logbuf;
    std::ofstream of;
    if (options.GetOutputFilename() == "-") {
        buf = std::cout.rdbuf();
        if (!options.GetOutputXml() && !options.GetOutputJSON()) {
            logbuf = std::cout.rdbuf();
        }
        else {
          logbuf = 0;
        }
    }
    else {
        of.open(options.GetOutputFilename().c_str(), std::ios::out | std::ios::binary);
        buf = of.rdbuf();
        logbuf = std::cout.rdbuf();
    }

    std::ostream out(buf);
    std::ostream log(logbuf);
    if (!out) {
        std::ostringstream stream;
        stream
            << "Error: Can't open file: "
            << options.GetOutputFilename()
            << std::endl;
        throw std::runtime_error(stream.str().c_str());
    }

    log << "Loading and hashing files ... " << std::flush;

    if (options.GetOutputXml()) {
        out
            << "<?xml version=\"1.0\"?>"
            << std::endl
            << "<duplo>"
            << std::endl;
    }

    auto json_out = options.GetOutputJSON() ? std::optional(json()) : std::nullopt;

    auto lines = FileSystem::LoadFileList(options.GetListFilename());
    auto [sourceFiles, matrix, files, locsTotal] = LoadSourceFiles(
        lines,
        options.GetMinChars(),
        options.GetIgnorePrepStuff(),
        log);
    auto numFilesToCheck = options.GetFilesToCheck() > 0 ? std::min(options.GetFilesToCheck(), sourceFiles.size()): sourceFiles.size();

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
                out,
                json_out);

        // files to compare are those that have matching lines
        for (unsigned j = i + 1; j < sourceFiles.size(); j++) {
            const auto& right = sourceFiles[j];
            if ((!options.GetIgnoreSameFilename() || !StringUtil::IsSameFilename(left, right))
                && matchingFiles.find(&right.GetFilename()) != matchingFiles.end()) {
                processResult
                    << Process(
                        left,
                        right,
                        matrix,
                        options,
                        out,
                        json_out);
            }
        }

        if (!options.GetOutputXml() && !options.GetOutputJSON()) {
            if (processResult.Blocks() > 0) {
                log
                    << left.GetFilename()
                    << " found: " << processResult.Blocks() << " block(s)" << std::endl;
            } else {
                log
                    << left.GetFilename()
                    << " nothing found." << std::endl;
            }
        }

        processResultTotal << processResult;
    }

    if (options.GetOutputXml()) {
        out
            << "</duplo>"
            << std::endl;
    } else if (json_out) {
        out << json_out->dump(2);
    } else {
        out
            << "Configuration:"
            << std::endl
            << "  Number of files: "
            << files
            << std::endl
            << "  Minimal block size: "
            << options.GetMinBlockSize()
            << std::endl
            << "  Minimal characters in line: "
            << options.GetMinChars()
            << std::endl
            << "  Ignore preprocessor directives: "
            << options.GetIgnorePrepStuff()
            << std::endl
            << "  Ignore same filenames: "
            << options.GetIgnoreSameFilename()
            << std::endl
            << std::endl
            << "Results:"
            << std::endl
            << "  Lines of code: "
            << locsTotal
            << std::endl
            << "  Duplicate lines of code: "
            << processResultTotal.DuplicateLines()
            //<< " ("
            //<< 100 * processResultTotal.DuplicateLines() / locsTotal
            //<< "%)"
            << std::endl
            << "  Total "
            << processResultTotal.Blocks()
            << " duplicate block(s) found."
            << std::endl
            << std::endl;
    }

    return processResultTotal.Blocks() > 0
        ? EXIT_FAILURE
        : EXIT_SUCCESS;
}
