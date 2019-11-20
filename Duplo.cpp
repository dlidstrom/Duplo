#include "Duplo.h"
#include "ArgumentParser.h"
#include "HashUtil.h"
#include "SourceFile.h"
#include "SourceLine.h"
#include "StringUtil.h"
#include "TextFile.h"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <sstream>
#include <time.h>

namespace {
    bool isSameFilename(const std::string& filename1, const std::string& filename2) {
        return StringUtil::GetFilenamePart(filename1) == StringUtil::GetFilenamePart(filename2);
    }
}

Duplo::Duplo(
    const std::string& listFilename,
    unsigned int minBlockSize,
    unsigned int blockPercentThreshold,
    unsigned int minChars,
    bool ignorePrepStuff,
    bool ignoreSameFilename,
    bool xml)
    : m_listFilename(listFilename),
      m_minBlockSize(minBlockSize),
      m_blockPercentThreshold(blockPercentThreshold),
      m_minChars(minChars),
      m_ignorePrepStuff(ignorePrepStuff),
      m_ignoreSameFilename(ignoreSameFilename),
      m_maxLinesPerFile(0),
      m_duplicateLines(0),
      m_xml(xml) {
}

void Duplo::ReportSeq(int line1, int line2, int count, const SourceFile& source1, const SourceFile& source2, std::ostream& outFile) {
    if (m_xml) {
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
            m_duplicateLines++;
        }

        outFile << "        </lines>" << std::endl;
        outFile << "    </set>" << std::endl;
    } else {
        outFile << source1.GetFilename() << "(" << source1.GetLine(line1).GetLineNumber() << ")" << std::endl;
        outFile << source2.GetFilename() << "(" << source2.GetLine(line2).GetLineNumber() << ")" << std::endl;
        for (int j = 0; j < count; j++) {
            outFile << source1.GetLine(j + line1).GetLine() << std::endl;
            m_duplicateLines++;
        }

        outFile << std::endl;
    }
}

int Duplo::Process(const SourceFile& pSource1, const SourceFile& pSource2, std::ostream& outFile) {
    unsigned m = pSource1.GetNumOfLines();
    unsigned n = pSource2.GetNumOfLines();

    // Reset matrix data
    std::fill(std::begin(m_matrix), std::begin(m_matrix) + m * n, MatchType::NONE);

    // Compute matrix
    for (unsigned y = 0; y < m; y++) {
        auto& line = pSource1.GetLine(y);
        for (unsigned x = 0; x < n; x++) {
            if (line == pSource2.GetLine(x)) {
                m_matrix[x + n * y] = MatchType::MATCH;
            }
        }
    }

    // support reporting filtering by both:
    // - "lines of code duplicated", &
    // - "percentage of file duplicated"
    unsigned lMinBlockSize = std::max(
        m_minBlockSize,
        std::min(
            m_minBlockSize,
            (std::max(n, m) * 100) / m_blockPercentThreshold));

    int blocks = 0;

    // Scan vertical part
    for (unsigned y = 0; y < m; y++) {
        unsigned seqLen = 0;
        int maxX = std::min(n, m - y);
        for (int x = 0; x < maxX; x++) {
            if (m_matrix[x + n * (y + x)] == MatchType::MATCH) {
                seqLen++;
            } else {
                if (seqLen >= lMinBlockSize) {
                    int line1 = y + x - seqLen;
                    int line2 = x - seqLen;
                    if (line1 != line2 || pSource1 != pSource2) {
                        ReportSeq(line1, line2, seqLen, pSource1, pSource2, outFile);
                        blocks++;
                    }
                }
                seqLen = 0;
            }
        }

        if (seqLen >= lMinBlockSize) {
            int line1 = m - seqLen;
            int line2 = n - seqLen;
            if (line1 != line2 || pSource1 != pSource2) {
                ReportSeq(line1, line2, seqLen, pSource1, pSource2, outFile);
                blocks++;
            }
        }
    }

    if (pSource1 != pSource2) {
        // Scan horizontal part
        for (unsigned x = 1; x < n; x++) {
            unsigned seqLen = 0;
            int maxY = std::min(m, n - x);
            for (int y = 0; y < maxY; y++) {
                if (m_matrix[x + y + n * y] == MatchType::MATCH) {
                    seqLen++;
                } else {
                    if (seqLen >= lMinBlockSize) {
                        ReportSeq(y - seqLen, x + y - seqLen, seqLen, pSource1, pSource2, outFile);
                        blocks++;
                    }
                    seqLen = 0;
                }
            }

            if (seqLen >= lMinBlockSize) {
                ReportSeq(m - seqLen, n - seqLen, seqLen, pSource1, pSource2, outFile);
                blocks++;
            }
        }
    }

    return blocks;
}

void Duplo::Run(std::string outputFileName) {
    std::ofstream outfile(outputFileName.c_str(), std::ios::out | std::ios::binary);

    if (m_xml) {
        outfile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
        outfile << "<?xml-stylesheet href=\"duplo.xsl\" type=\"text/xsl\"?>" << std::endl;
        outfile << "<duplo version=\"" << VERSION << "\">" << std::endl;
        outfile << "    <check Min_block_size=\"" << m_minBlockSize << "\" Min_char_line=\"" << m_minChars << "\" Ignore_prepro=\"" << (m_ignorePrepStuff ? "true" : "false") << "\" Ignore_same_filename=\"" << (m_ignoreSameFilename ? "true" : "false") << "\">" << std::endl;
    }

    if (!outfile.is_open()) {
        std::cout << "Error: Can't open file: " << outputFileName << std::endl;
        return;
    }

    double duration;
    clock_t start = clock();

    std::cout << "Loading and hashing files ... " << std::flush;

    std::vector<SourceFile> sourceFiles;

    TextFile listOfFiles(m_listFilename);
    auto lines = listOfFiles.ReadLines(true);

    int files = 0;
    unsigned long locsTotal = 0;
    typedef std::tuple<unsigned, std::string> FileLength;
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
            SourceFile pSourceFile(lines[i], m_minChars, m_ignorePrepStuff);
            unsigned numLines = pSourceFile.GetNumOfLines();
            if (numLines > 0) {
                files++;
                sourceFiles.push_back(std::move(pSourceFile));
                locsTotal += numLines;
                if (m_maxLinesPerFile < numLines) {
                    m_maxLinesPerFile = numLines;
                }

                // keep 10 worst case files
                if (longestFiles.size() < 10) {
                    addSorted(numLines, lines[i]);
                } else {
                    auto [l, _] = longestFiles.back();
                    if (l < numLines) {
                        addSorted(numLines, lines[i]);
                    }
                }
            }
        }
    }

    std::cout
        << lines.size()
        << " done.\n\n";

    if (m_maxLinesPerFile * m_maxLinesPerFile > m_matrix.max_size()) {
        std::ostringstream stream;
        stream
            << "Some files have too many lines. You can have files with approximately "
            << std::sqrt(m_matrix.max_size())
            << " lines at most." << std::endl
            << "Longest files:" << std::endl;
        for (auto [l, f] : longestFiles) {
            stream << l << ": " << f << std::endl;
        }

        throw std::exception(stream.str().c_str());
    }

    // Generate matrix large enough for all files
    try {
        m_matrix.resize(m_maxLinesPerFile * m_maxLinesPerFile);
    }
    catch (const std::bad_alloc& ex) {
        std::ostringstream stream;
        stream
            << ex.what() << std::endl
            << "Longest files:" << std::endl;
        for (auto v : longestFiles) {
            stream << std::get<1>(v) << ": " << std::get<0>(v) << std::endl;
        }

        throw std::exception(stream.str().c_str());
    }

    int blocksTotal = 0;

    // Compare each file with each other
    for (unsigned i = 0; i < sourceFiles.size(); i++) {
        std::cout << sourceFiles[i].GetFilename();
        int blocks = 0;

        blocks += Process(sourceFiles[i], sourceFiles[i], outfile);
        for (unsigned j = i + 1; j < sourceFiles.size(); j++) {
            if (!m_ignoreSameFilename || !isSameFilename(sourceFiles[i].GetFilename(), sourceFiles[j].GetFilename())) {
                blocks += Process(sourceFiles[i], sourceFiles[j], outfile);
            }
        }

        if (blocks > 0) {
            std::cout << " found: " << blocks << " block(s)" << std::endl;
        } else {
            std::cout << " nothing found." << std::endl;
        }

        blocksTotal += blocks;
    }

    clock_t finish = clock();
    duration = (double)(static_cast<long long>(finish) - start) / CLOCKS_PER_SEC;
    std::cout << "Time: " << duration << " seconds" << std::endl;

    if (m_xml) {
        outfile
            << "        <summary Num_files=\""
            << files
            << "\" Duplicate_blocks=\""
            << blocksTotal
            << "\" Total_lines_of_code=\""
            << locsTotal
            << "\" Duplicate_lines_of_code=\""
            << m_duplicateLines
            << "\" Time=\""
            << duration
            << "\"/>"
            << std::endl;
        outfile << "    </check>" << std::endl;
        outfile << "</duplo>" << std::endl;
    } else {
        outfile << "Configuration: " << std::endl;
        outfile << "  Number of files: " << files << std::endl;
        outfile << "  Minimal block size: " << m_minBlockSize << std::endl;
        outfile << "  Minimal characters in line: " << m_minChars << std::endl;
        outfile << "  Ignore preprocessor directives: " << m_ignorePrepStuff << std::endl;
        outfile << "  Ignore same filenames: " << m_ignoreSameFilename << std::endl;
        outfile << std::endl;
        outfile << "Results: " << std::endl;
        outfile << "  Lines of code: " << locsTotal << std::endl;
        outfile << "  Duplicate lines of code: " << m_duplicateLines << std::endl;
        outfile << "  Total " << blocksTotal << " duplicate block(s) found." << std::endl
                << std::endl;
        //outfile << "  Time: " << duration << " seconds" << std::endl;
    }
}
