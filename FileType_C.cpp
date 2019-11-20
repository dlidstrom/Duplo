#include "FileType_C.h"
#include "CstyleUtils.h"
#include "SourceLine.h"

FileType_C::FileType_C(bool ignorePrepStuff, unsigned minChars)
    : FileTypeBase(ignorePrepStuff, minChars) {
}

std::vector<SourceLine> FileType_C::GetCleanedSourceLines(const std::vector<std::string>&) const {
    return std::vector<SourceLine>();
}

std::string FileType_C::GetCleanLine(const std::string& line) const {
    return CstyleUtils::RemoveSingleLineComments(line);
}

bool FileType_C::IsPreprocessorDirective(const std::string& line) const {
    return line[0] == '#';
}
