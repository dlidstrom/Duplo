#include "Duplo.h"
#include "ArgumentParser.h"
#include "HashUtil.h"
#include "Options.h"
#include "SourceFile.h"
#include "SourceLine.h"
#include "StringUtil.h"
#include "TextFile.h"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <ctime>
#include <fstream>
#include <sstream>
#include <tuple>
#include <unordered_map>
#include <unordered_set>

enum class MatchType : unsigned char {
    NONE,
    MATCH
};
typedef std::tuple<unsigned, std::string> FileLength;
typedef const std::string* StringPtr;
typedef std::unordered_map<unsigned long, std::vector<StringPtr>> HashToFiles;

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
    bool IsSameFilename(const SourceFile& left, const SourceFile& right) {
        return StringUtil::GetFilenamePart(left.GetFilename()) == StringUtil::GetFilenamePart(right.GetFilename());
    }

    std::vector<std::string> LoadFileList(const std::string& listFilename) {
        if (listFilename == "-") {
            std::vector<std::string> lines;
            std::string line;
            while (std::getline(std::cin, line)) {
                lines.push_back(line);
            }

            return lines;
        } else {
            TextFile textFile(listFilename);
            auto lines = textFile.ReadLines(true);
            return lines;
        }
    }

    std::tuple<std::vector<SourceFile>, std::vector<MatchType>, unsigned, unsigned> LoadSourceFiles(
        const std::vector<std::string>& lines,
        unsigned minChars,
        bool ignorePrepStuff) {

        std::vector<SourceFile> sourceFiles;
        std::vector<MatchType> matrix;
        unsigned maxLinesPerFile = 0;
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
        for (unsigned i = 0; i < lines.size(); i++) {
            if (lines[i].size() > 5) {
                SourceFile sourceFile(lines[i], minChars, ignorePrepStuff);
                unsigned numLines = sourceFile.GetNumOfLines();
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
                        auto [l, r] = longestFiles.back();
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
            for (auto [l, f] : longestFiles) {
                stream << l << ": " << f << std::endl;
            }

            throw std::runtime_error(stream.str().c_str());
        }

        std::cout
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
            for (auto [l, f] : longestFiles) {
                stream << l << ": " << f << std::endl;
            }

            throw std::runtime_error(stream.str().c_str());
        }

        return std::tuple(std::move(sourceFiles), matrix, files, locsTotal);
    }

    unsigned ReportSeq(
        int line1,
        int line2,
        int count,
        bool xml,
        const SourceFile& source1,
        const SourceFile& source2,
        std::ostream& outFile) {
        unsigned duplicateLines = 0;
        if (xml) {
            outFile << "    <set LineCount=\"" << count << "\">" << std::endl;
            outFile << "        <block SourceFile=\"" << source1.GetFilename() << "\" StartLineNumber=\"" << source1.GetLine(line1).GetLineNumber() << "\"/>" << std::endl;
            outFile << "        <block SourceFile=\"" << source2.GetFilename() << "\" StartLineNumber=\"" << source2.GetLine(line2).GetLineNumber() << "\"/>" << std::endl;
            outFile << "        <lines xml:space=\"preserve\">" << std::endl;
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

                outFile << "            <line Text=\"" << tmpstr << "\"/>" << std::endl;
                duplicateLines++;
            }

            outFile << "        </lines>" << std::endl;
            outFile << "    </set>" << std::endl;
        } else {
            outFile << source1.GetFilename() << "(" << source1.GetLine(line1).GetLineNumber() << ")" << std::endl;
            outFile << source2.GetFilename() << "(" << source2.GetLine(line2).GetLineNumber() << ")" << std::endl;
            for (int j = 0; j < count; j++) {
                outFile << source1.GetLine(j + line1).GetLine() << std::endl;
                duplicateLines++;
            }

            outFile << std::endl;
        }

        return duplicateLines;
    }

    ProcessResult Process(
        const SourceFile& source1,
        const SourceFile& source2,
        std::vector<MatchType>& matrix,
        const Options& options,
        std::ostream& outFile) {
        unsigned m = source1.GetNumOfLines();
        unsigned n = source2.GetNumOfLines();

        // Reset matrix data
        std::fill(std::begin(matrix), std::begin(matrix) + m * n, MatchType::NONE);

        // Compute matrix
        for (unsigned y = 0; y < m; y++) {
            auto& line = source1.GetLine(y);
            for (unsigned x = 0; x < n; x++) {
                if (line == source2.GetLine(x)) {
                    matrix[x + n * y] = MatchType::MATCH;
                }
            }
        }

        // support reporting filtering by both:
        // - "lines of code duplicated", &
        // - "percentage of file duplicated"
        unsigned lMinBlockSize = std::max(
            options.GetMinBlockSize(),
            std::min(
                options.GetMinBlockSize(),
                (std::max(n, m) * 100) / options.GetBlockPercentThreshold()));

        unsigned blocks = 0;
        unsigned duplicateLines = 0;

        // Scan vertical part
        for (unsigned y = 0; y < m; y++) {
            unsigned seqLen = 0;
            int maxX = std::min(n, m - y);
            for (int x = 0; x < maxX; x++) {
                if (matrix[x + n * (y + x)] == MatchType::MATCH) {
                    seqLen++;
                } else {
                    if (seqLen >= lMinBlockSize) {
                        int line1 = y + x - seqLen;
                        int line2 = x - seqLen;
                        if (line1 != line2 || source1 != source2) {
                            duplicateLines +=
                                ReportSeq(
                                    line1,
                                    line2,
                                    seqLen,
                                    options.GetOutputXml(),
                                    source1,
                                    source2,
                                    outFile);
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
                    duplicateLines +=
                        ReportSeq(
                            line1,
                            line2,
                            seqLen,
                            options.GetOutputXml(),
                            source1,
                            source2,
                            outFile);
                    blocks++;
                }
            }
        }

        if (source1 != source2) {
            // Scan horizontal part
            for (unsigned x = 1; x < n; x++) {
                unsigned seqLen = 0;
                int maxY = std::min(m, n - x);
                for (int y = 0; y < maxY; y++) {
                    if (matrix[x + y + n * y] == MatchType::MATCH) {
                        seqLen++;
                    } else {
                        if (seqLen >= lMinBlockSize) {
                            duplicateLines +=
                                ReportSeq(
                                    y - seqLen,
                                    x + y - seqLen,
                                    seqLen,
                                    options.GetOutputXml(),
                                    source1,
                                    source2,
                                    outFile);
                            blocks++;
                        }
                        seqLen = 0;
                    }
                }

                if (seqLen >= lMinBlockSize) {
                    duplicateLines +=
                        ReportSeq(
                            m - seqLen,
                            n - seqLen,
                            seqLen,
                            options.GetOutputXml(),
                            source1,
                            source2,
                            outFile);
                    blocks++;
                }
            }
        }

        return ProcessResult(blocks, duplicateLines);
    }
}

void Duplo::Run(const Options& options) {
    std::ofstream outfile(
        options.GetOutputFilename().c_str(), std::ios::out | std::ios::binary);
    if (!outfile) {
        std::ostringstream stream;
        stream
            << "Error: Can't open file: "
            << options.GetOutputFilename()
            << std::endl;
        throw std::runtime_error(stream.str().c_str());
    }

    std::cout << "Loading and hashing files ... " << std::flush;

    if (options.GetOutputXml()) {
        outfile
            << "<?xml version=\"1.0\"?>"
            << std::endl
            << "<duplo>"
            << std::endl;
    }

    auto lines = LoadFileList(options.GetListFilename());
    auto [sourceFiles, matrix, files, locsTotal] =
        LoadSourceFiles(lines, options.GetMinChars(), options.GetIgnorePrepStuff());

    // hash maps
    HashToFiles hashToFiles;
    for (const auto& s : sourceFiles) {
        for (size_t i = 0; i < s.GetNumOfLines(); i++) {
            hashToFiles[s.GetLine(i).GetHash()].push_back(&s.GetFilename());
        }
    }

    // Compare each file with each other
    ProcessResult processResultTotal;
    for (unsigned i = 0; i < sourceFiles.size(); i++) {
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

        std::cout << left.GetFilename();
        ProcessResult processResult =
            Process(
                left,
                left,
                matrix,
                options,
                outfile);

        // files to compare are those that have matching lines
        for (unsigned j = i + 1; j < sourceFiles.size(); j++) {
            const auto& right = sourceFiles[j];
            if ((!options.GetIgnoreSameFilename() || !IsSameFilename(left, right))
                && matchingFiles.find(&right.GetFilename()) != matchingFiles.end()) {
                processResult
                    << Process(
                        left,
                        right,
                        matrix,
                        options,
                        outfile);
            }
        }

        if (processResult.Blocks() > 0) {
            std::cout << " found: " << processResult.Blocks() << " block(s)" << std::endl;
        } else {
            std::cout << " nothing found." << std::endl;
        }

        processResultTotal << processResult;
    }

    if (options.GetOutputXml()) {
        outfile
            << "</duplo>"
            << std::endl;
    } else {
        outfile
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
}
