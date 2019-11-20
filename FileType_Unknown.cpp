#include "FileType_Unknown.h"
#include "SourceLine.h"

FileType_Unknown::FileType_Unknown(unsigned minChars)
    : FileTypeBase(false, minChars) {
}

std::vector<SourceLine> FileType_Unknown::GetCleanedSourceLines(const std::vector<std::string>&) const {
    return std::vector<SourceLine>();
}

std::string FileType_Unknown::GetCleanLine(const std::string& line) const {
    return line;
}

bool FileType_Unknown::IsPreprocessorDirective(const std::string&) const {
    return false;
}
