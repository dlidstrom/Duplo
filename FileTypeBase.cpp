#include "FileTypeBase.h"
#include "StringUtil.h"

#include <algorithm>

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
        tmp.size() >= m_minChars && std::find_if(std::begin(tmp), std::end(tmp), isalpha) != std::end(tmp);
    return isSourceLine;
}
