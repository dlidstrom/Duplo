#include "FileTypeBase.h"
#include "ILineFilter.h"
#include "SourceLine.h"
#include "StringUtil.h"

#include <algorithm>
#include <cctype>

FileTypeBase::FileTypeBase(bool ignorePrepStuff, unsigned minChars)
    : m_ignorePrepStuff(ignorePrepStuff),
      m_minChars(minChars) {
}

bool FileTypeBase::IsSourceLine(const std::string& line) const {
    std::string tmp = StringUtil::ToLower(StringUtil::Trim(line));

    // filter min size lines
    if (tmp.size() < m_minChars) {
        return false;
    }

    if (m_ignorePrepStuff && IsPreprocessorDirective(tmp))
        return false;

    // must be at least one alpha-numeric character
    bool isSourceLine =
        tmp.size() >= m_minChars
        && std::find_if(
            std::begin(tmp),
            std::end(tmp),
            [](int c) { return std::isalpha(c); }) != std::end(tmp);
    return isSourceLine;
}

std::vector<SourceLine> FileTypeBase::GetCleanedSourceLines(const std::vector<std::string>& lines) const {
    auto lineFilter = CreateLineFilter();
    std::vector<SourceLine> filteredLines;
    for (std::vector<std::string>::size_type i = 0; i < lines.size(); i++) {
        auto filteredLine = GetCleanLine(lineFilter->ProcessSourceLine(lines[i]));
        if (IsSourceLine(filteredLine)) {
            filteredLines.emplace_back(filteredLine, i);
        }
    }

    return filteredLines;
}
