#ifndef _SOURCELINE_H_
#define _SOURCELINE_H_

#include <string>

class SourceLine {
    std::string m_line;
    int m_lineNumber;
    unsigned long m_hash;

public:
    /**
     * Creates a new text file. The file is accessed relative to current directory.
     */
    SourceLine(const std::string& line, int lineNumber);

    int GetLineNumber() const;
    const std::string& GetLine() const;
    unsigned long GetHash() const;
    bool operator==(const SourceLine& other) const;
};

#endif
