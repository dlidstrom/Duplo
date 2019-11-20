#include "FileType_H.h"
#include "CstyleUtils.h"
#include "SourceLine.h"

FileType_H::FileType_H(bool ignorePrepStuff, unsigned minChars)
    : FileTypeBase(ignorePrepStuff, minChars) {
}

std::vector<SourceLine> FileType_H::GetCleanedSourceLines(const std::vector<std::string>&) const {
    return std::vector<SourceLine>();
}

std::string FileType_H::GetCleanLine(const std::string& line) const {
    return CstyleUtils::RemoveSingleLineComments(line);
}

bool FileType_H::IsPreprocessorDirective(const std::string& line) const {
    return line[0] == '#';
}
