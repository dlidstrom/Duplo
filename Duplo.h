#ifndef _DUPLO_H_
#define _DUPLO_H_

#include <string>
#include <iostream>
#include <vector>

class SourceFile;

const std::string VERSION = "0.4.0";

class Duplo {
    std::string m_listFileName;
    unsigned int m_minBlockSize;
    unsigned int m_blockPercentThreshold;
    unsigned int m_minChars;
    bool m_ignorePrepStuff;
    bool m_ignoreSameFilename;
    unsigned m_maxLinesPerFile;
    int m_DuplicateLines;
    bool m_Xml;

    enum class MatchType : unsigned char {
        NONE,
        MATCH
    };
    std::vector<MatchType> m_pMatrix;

    void reportSeq(int line1, int line2, int count, const SourceFile& pSource1, const SourceFile& pSource2, std::ostream& outFile);
    int process(const SourceFile& pSource1, const SourceFile& pSource2, std::ostream& outFile);

    const std::string getFilenamePart(const std::string& fullpath) const;
    bool isSameFilename(const std::string& filename1, const std::string& filename2) const;

public:
    Duplo(
        const std::string& listFileName,
        unsigned int blockPercentThreshold,
        unsigned int minBlockSize,
        unsigned int minChars,
        bool ignorePrepStuff, bool ignoreSameFilename, bool Xml);
    void run(std::string outputFileName);
};

#endif
