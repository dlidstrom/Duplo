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

Duplo::Duplo(
    const std::string& listFileName,
    unsigned int minBlockSize,
    unsigned int blockPercentThreshold,
    unsigned int minChars,
    bool ignorePrepStuff,
    bool ignoreSameFilename,
    bool Xml)
    : m_listFileName(listFileName),
      m_minBlockSize(minBlockSize),
      m_blockPercentThreshold(blockPercentThreshold),
      m_minChars(minChars),
      m_ignorePrepStuff(ignorePrepStuff),
      m_ignoreSameFilename(ignoreSameFilename),
      m_maxLinesPerFile(0),
      m_DuplicateLines(0),
      m_Xml(Xml),
      m_pMatrix(NULL) {
}

void Duplo::reportSeq(int line1, int line2, int count, const SourceFile& pSource1, const SourceFile& pSource2, std::ostream& outFile) {
    if (m_Xml) {
        outFile << "    <set LineCount=\"" << count << "\">" << std::endl;
        outFile << "        <block SourceFile=\"" << pSource1.getFilename() << "\" StartLineNumber=\"" << pSource1.getLine(line1).getLineNumber() << "\"/>" << std::endl;
        outFile << "        <block SourceFile=\"" << pSource2.getFilename() << "\" StartLineNumber=\"" << pSource2.getLine(line2).getLineNumber() << "\"/>" << std::endl;
        outFile << "        <lines xml:space=\"preserve\">" << std::endl;
        for (int j = 0; j < count; j++) {
            // replace various characters/ strings so that it doesn't upset the XML parser
            std::string tmpstr = pSource1.getLine(j + line1).getLine();

            // " --> '
            StringUtil::StrSub(tmpstr, "\'", "\"", -1);

            // & --> &amp;
            StringUtil::StrSub(tmpstr, "&amp;", "&", -1);

            // < --> &lt;
            StringUtil::StrSub(tmpstr, "&lt;", "<", -1);

            // > --> &gt;
            StringUtil::StrSub(tmpstr, "&gt;", ">", -1);

            outFile << "            <line Text=\"" << tmpstr << "\"/>" << std::endl;
            m_DuplicateLines++;
        }
        outFile << "        </lines>" << std::endl;
        outFile << "    </set>" << std::endl;
    } else {
        outFile << pSource1.getFilename() << "(" << pSource1.getLine(line1).getLineNumber() << ")" << std::endl;
        outFile << pSource2.getFilename() << "(" << pSource2.getLine(line2).getLineNumber() << ")" << std::endl;
        for (int j = 0; j < count; j++) {
            outFile << pSource1.getLine(j + line1).getLine() << std::endl;
            m_DuplicateLines++;
        }
        outFile << std::endl;
    }
}

int Duplo::process(const SourceFile& pSource1, const SourceFile& pSource2, std::ostream& outFile) {
    const unsigned int m = pSource1.getNumOfLines();
    const unsigned int n = pSource2.getNumOfLines();

    // Reset matrix data
    std::fill(m_pMatrix.begin(), m_pMatrix.begin() + m * n, MatchType::NONE);

    // Compute matrix
    for (unsigned int y = 0; y < m; y++) {
        const SourceLine& pSLine = pSource1.getLine(y);
        for (unsigned int x = 0; x < n; x++) {
            if (pSLine == pSource2.getLine(x)) {
                m_pMatrix[x + n * y] = MatchType::MATCH;
            }
        }
    }

    // support reporting filtering by both:
    // - "lines of code duplicated", &
    // - "percentage of file duplicated"
    const unsigned int lMinBlockSize = std::max(
        m_minBlockSize,
        std::min(
            m_minBlockSize,
            (std::max(n, m) * 100) / m_blockPercentThreshold));

    int blocks = 0;

    // Scan vertical part
    for (unsigned int y = 0; y < m; y++) {
        unsigned int seqLen = 0;
        int maxX = std::min(n, m - y);
        for (int x = 0; x < maxX; x++) {
            if (m_pMatrix[x + n * (y + x)] == MatchType::MATCH) {
                seqLen++;
            } else {
                if (seqLen >= lMinBlockSize) {
                    int line1 = y + x - seqLen;
                    int line2 = x - seqLen;
                    if (line1 != line2 || pSource1 != pSource2) {
                        reportSeq(line1, line2, seqLen, pSource1, pSource2, outFile);
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
                reportSeq(line1, line2, seqLen, pSource1, pSource2, outFile);
                blocks++;
            }
        }
    }

    if (pSource1 != pSource2) {
        // Scan horizontal part
        for (unsigned int x = 1; x < n; x++) {
            unsigned int seqLen = 0;
            int maxY = std::min(m, n - x);
            for (int y = 0; y < maxY; y++) {
                if (m_pMatrix[x + y + n * y] == MatchType::MATCH) {
                    seqLen++;
                } else {
                    if (seqLen >= lMinBlockSize) {
                        reportSeq(y - seqLen, x + y - seqLen, seqLen, pSource1, pSource2, outFile);
                        blocks++;
                    }
                    seqLen = 0;
                }
            }

            if (seqLen >= lMinBlockSize) {
                reportSeq(m - seqLen, n - seqLen, seqLen, pSource1, pSource2, outFile);
                blocks++;
            }
        }
    }

    return blocks;
}

const std::string Duplo::getFilenamePart(const std::string& fullpath) const {
    std::string path = StringUtil::Substitute('\\', '/', fullpath);

    std::string filename = path;

    std::string::size_type idx = path.rfind('/');
    if (idx != std::string::npos) {
        filename = path.substr(idx + 1, path.size() - idx - 1);
    }

    return filename;
}

bool Duplo::isSameFilename(const std::string& filename1, const std::string& filename2) const {
    return getFilenamePart(filename1) == getFilenamePart(filename2);
}

void Duplo::run(std::string outputFileName) {
    std::ofstream outfile(outputFileName.c_str(), std::ios::out | std::ios::binary);

    if (m_Xml) {
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

    TextFile listOfFiles(m_listFileName);
    auto lines = listOfFiles.readLines(true);

    int files = 0;
    unsigned long locsTotal = 0;
    typedef std::tuple<int, std::string> FileLength;
    std::vector<std::tuple<int, std::string>> longestFiles;
    auto addSorted = [&longestFiles](int numLines, const std::string& filename) {
        longestFiles.emplace_back(numLines, filename);
        std::sort(
            longestFiles.begin(),
            longestFiles.end(),
            [](auto l, auto r) { return std::get<0>(l) > std::get<0>(r); });
        if (longestFiles.size() > 10)
            longestFiles.resize(10);
    };

    // Create vector with all source files
    for (unsigned i = 0; i < lines.size(); i++) {
        if (lines[i].size() > 5) {
            SourceFile pSourceFile(lines[i], m_minChars, m_ignorePrepStuff);
            unsigned numLines = pSourceFile.getNumOfLines();
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
                    unsigned l = std::get<0>(longestFiles.back());
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

    if (m_maxLinesPerFile * m_maxLinesPerFile > m_pMatrix.max_size()) {
        std::ostringstream stream;
        stream
            << "Some files have too many lines. You can have files with approximately "
            << std::sqrt(m_pMatrix.max_size())
            << " lines at most." << std::endl
            << "Longest files:" << std::endl;
        for (auto v : longestFiles) {
            stream << std::get<1>(v) << ": " << std::get<0>(v) << std::endl;
        }

        throw std::exception(stream.str().c_str());
    }

    // Generate matrix large enough for all files
    try {
        m_pMatrix.resize(m_maxLinesPerFile * m_maxLinesPerFile);
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
    for (int i = 0; i < (int)sourceFiles.size(); i++) {
        std::cout << sourceFiles[i].getFilename();
        int blocks = 0;

        blocks += process(sourceFiles[i], sourceFiles[i], outfile);
        for (int j = i + 1; j < (int)sourceFiles.size(); j++) {
            if (!m_ignoreSameFilename || !isSameFilename(sourceFiles[i].getFilename(), sourceFiles[j].getFilename())) {
                blocks += process(sourceFiles[i], sourceFiles[j], outfile);
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

    if (m_Xml) {
        outfile << "        <summary Num_files=\"" << files << "\" Duplicate_blocks=\"" << blocksTotal << "\" Total_lines_of_code=\"" << locsTotal << "\" Duplicate_lines_of_code=\"" << m_DuplicateLines << "\" Time=\"" << duration << "\"/>" << std::endl;
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
        outfile << "  Duplicate lines of code: " << m_DuplicateLines << std::endl;
        outfile << "  Total " << blocksTotal << " duplicate block(s) found." << std::endl
                << std::endl;
        //outfile << "  Time: " << duration << " seconds" << std::endl;
    }
}
