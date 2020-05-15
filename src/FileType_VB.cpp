#include "FileType_VB.h"
#include "CstyleUtils.h"
#include "NoopLineFilter.h"
#include "SourceLine.h"

#include <cstring>

FileType_VB::FileType_VB(bool ignorePrepStuff, unsigned minChars)
    : FileTypeBase(ignorePrepStuff, minChars) {
}

ILineFilterPtr FileType_VB::CreateLineFilter() const {
    return std::make_shared<NoopLineFilter>();
}

std::string FileType_VB::GetCleanLine(const std::string& line) const {
    std::string cleanedLine(line.substr(0, line.find_first_of('\'')));
    return cleanedLine;
}

bool FileType_VB::IsPreprocessorDirective(const std::string& line) const {
    // look for other markers to avoid
    const char* markers[] = { "imports" };

    for (auto v : markers) {
        if (line.find(v, 0, std::strlen(v)) != std::string::npos)
            return true;
    }

    return false;
}
