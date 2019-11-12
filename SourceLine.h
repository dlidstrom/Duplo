#ifndef _SOURCELINE_H_
#define _SOURCELINE_H_

#include <string>
#include <vector>

class SourceLine {
protected:
    std::string m_line;
    int m_lineNumber;

public:
    SourceLine(std::string& line, int lineNumber);

    long long m_hashHigh;
    long long m_hashLow;

    int getLineNumber();
    std::string& getLine();
    bool equals(SourceLine* pLine);
};

#endif
