#include "FileType_Java.h"
#include "CstyleCommentsFilter.h"
#include "Utils.h"

#include <cstring>

FileType_Java::FileType_Java(bool ignorePrepStuff, unsigned minChars)
    : FileTypeBase(ignorePrepStuff, minChars) {
}

ILineFilterPtr FileType_Java::CreateLineFilter() const {
    return std::make_shared<CstyleCommentsLineFilter>();
}

std::string FileType_Java::GetCleanLine(const std::string& line) const {
    return CstyleUtils::RemoveSingleLineComments(line);
}

bool FileType_Java::IsPreprocessorDirective(const std::string& line) const {
    // look for other markers to avoid
    const char* markers[] = { "package", "import", "private", "protected", "public" };

    for (auto v : markers) {
        if (line.find(v, 0, std::strlen(v)) != std::string::npos)
            return true;
    }

    return false;
}
