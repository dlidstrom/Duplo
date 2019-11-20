#include "FileType_CS.h"
#include "CstyleUtils.h"
#include "SourceLine.h"

FileType_CS::FileType_CS(bool ignorePrepStuff, unsigned minChars)
    : FileTypeBase(ignorePrepStuff, minChars) {
}

std::vector<SourceLine> FileType_CS::GetCleanedSourceLines(const std::vector<std::string>&) const {
    return std::vector<SourceLine>();
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
    const std::string PreProc_CS[] = { "using", "private", "protected", "public" };

    for (int i = 0; i < 4; i++) {
        if (line.find(PreProc_CS[i].c_str(), 0, PreProc_CS[i].length()) != std::string::npos)
            return true;
    }

    return false;
}
