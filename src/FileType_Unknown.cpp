#include "FileType_Unknown.h"
#include "NoopLineFilter.h"
#include "SourceLine.h"

FileType_Unknown::FileType_Unknown(unsigned minChars)
    : FileTypeBase(false, minChars) {
}

ILineFilterPtr FileType_Unknown::CreateLineFilter() const {
    return std::make_shared<NoopLineFilter>();
}

std::string FileType_Unknown::GetCleanLine(const std::string& line) const {
    return line;
}

bool FileType_Unknown::IsPreprocessorDirective(const std::string&) const {
    return false;
}
