#include "SourceLine.h"
#include "HashUtil.h"
#include "SourceFile.h"

SourceLine::SourceLine(const std::string& line, int lineNumber) {
    m_line = line;
    m_lineNumber = lineNumber;

    std::string cleanLine;

    //Remove all white space and noise (tabs etc)
    for (int i = 0; i < (int)line.size(); i++) {
        if (line[i] > ' ') {
            cleanLine.push_back(line[i]);
        }
    }

    hash = HashUtil::Hash(cleanLine.c_str(), cleanLine.size());
}

int SourceLine::GetLineNumber() const {
    return m_lineNumber;
}

bool SourceLine::operator==(const SourceLine& other) const {
    return hash == other.hash;
}

const std::string& SourceLine::GetLine() const {
    return m_line;
}
