#include "FileType_CS.h"
#include "CstyleCommentsFilter.h"
#include "CstyleUtils.h"
#include "SourceLine.h"

#include <cstring>

FileType_CS::FileType_CS(bool ignorePrepStuff, unsigned minChars)
    : FileTypeBase(ignorePrepStuff, minChars) {
}

ILineFilterPtr FileType_CS::CreateLineFilter() const {
    return std::make_shared<CstyleCommentsLineFilter>();
}

std::string FileType_CS::GetCleanLine(const std::string& line) const {
    return CstyleUtils::RemoveSingleLineComments(line);
}

bool FileType_CS::IsPreprocessorDirective(const std::string& line) const {
    if (line[0] == '#')
        return true;

    // look for attribute
    if (line[0] == '[') {
        return true;
    }

    // look for other markers to avoid
    const char* markers[] = { "using", "private", "protected", "public" };

    for (auto v : markers) {
        if (line.find(v, 0, std::strlen(v)) != std::string::npos)
            return true;
    }

    return false;
}
