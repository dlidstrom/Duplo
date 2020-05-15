#include "FileType_S.h"
#include "CstyleUtils.h"
#include "NoopLineFilter.h"
#include "SourceLine.h"

#include <cstring>

FileType_S::FileType_S(bool ignorePrepStuff, unsigned minChars)
    : FileTypeBase(ignorePrepStuff, minChars) {
}

ILineFilterPtr FileType_S::CreateLineFilter() const {
    return std::make_shared<NoopLineFilter>();
}

std::string FileType_S::GetCleanLine(const std::string& line) const {
    std::string cleanedLine(line.substr(0, line.find_first_of(';')));
    return cleanedLine;
}

bool FileType_S::IsPreprocessorDirective(const std::string& line) const {
    // we can't deduplicate ret AFAIK
    const char* markers[] = { "ret" };

    for (auto v : markers) {
        if (line.find(v, 0, std::strlen(v)) != std::string::npos)
            return true;
    }

    return false;
}
