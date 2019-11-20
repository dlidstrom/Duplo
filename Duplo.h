#ifndef _DUPLO_H_
#define _DUPLO_H_

#include "Fwd.h"

#include <string>
#include <iostream>
#include <vector>

const std::string VERSION = "0.4.0";

class Duplo {
    std::string m_listFilename;
    unsigned int m_minBlockSize;
    unsigned int m_blockPercentThreshold;
    unsigned int m_minChars;
    bool m_ignorePrepStuff;
    bool m_ignoreSameFilename;
    unsigned m_maxLinesPerFile;
    int m_duplicateLines;
    bool m_xml;

    enum class MatchType : unsigned char {
        NONE,
        MATCH
    };
    std::vector<MatchType> m_matrix;

    void ReportSeq(int line1, int line2, int count, const SourceFile& source1, const SourceFile& source2, std::ostream& outFile);
    int Process(const SourceFile& source1, const SourceFile& source2, std::ostream& outFile);

public:
    Duplo(
        const std::string& listFilename,
        unsigned int blockPercentThreshold,
        unsigned int minBlockSize,
        unsigned int minChars,
        bool ignorePrepStuff, bool ignoreSameFilename, bool xml);
    void Run(std::string outputFileName);
};

#endif
