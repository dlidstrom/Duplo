#include "SourceLine.h"
#include "HashUtil.h"
#include "SourceFile.h"

/**
 * Creates a new text file. The file is accessed relative to current directory.
 */
SourceLine::SourceLine(std::string& line, int lineNumber) {
    m_line = line;
    m_lineNumber = lineNumber;

    std::string cleanLine;

    //Remove all white space and noise (tabs etc)
    for (int i = 0; i < (int)line.size(); i++) {
        if (line[i] > ' ') {
            cleanLine.push_back(line[i]);
        }
    }

    // MD5 hash
    long long* pDigest = (long long*)HashUtil::getMD5Sum((unsigned char*)cleanLine.c_str(), (int)cleanLine.size());
    m_hashHigh = pDigest[0];
    m_hashLow = pDigest[1];
}

int SourceLine::getLineNumber() {
    return m_lineNumber;
}

bool SourceLine::equals(SourceLine* pLine) {
    return (m_hashHigh == pLine->m_hashHigh && m_hashLow == pLine->m_hashLow);
}

std::string& SourceLine::getLine() {
    return m_line;
}
