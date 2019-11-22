#include "SourceLine.h"
#include "HashUtil.h"
#include "SourceFile.h"

#include <algorithm>

SourceLine::SourceLine(const std::string& line, int lineNumber) {
    m_line = line;
    m_lineNumber = lineNumber;

    std::string cleanLine;

    // Remove all white space and noise (tabs etc)
    std::copy_if(
        std::begin(line),
        std::end(line),
        std::back_inserter(cleanLine),
        [](char c) { return c > ' '; });
    m_hash = HashUtil::Hash(cleanLine.c_str(), cleanLine.size());
}

int SourceLine::GetLineNumber() const {
    return m_lineNumber;
}

bool SourceLine::operator==(const SourceLine& other) const {
    return m_hash == other.m_hash;
}

const std::string& SourceLine::GetLine() const {
    return m_line;
}

unsigned long SourceLine::GetHash() const {
    return m_hash;
}
