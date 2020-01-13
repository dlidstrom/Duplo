#include "FileType_C.h"
#include "CstyleCommentsFilter.h"
#include "CstyleUtils.h"
#include "SourceLine.h"

FileType_C::FileType_C(bool ignorePrepStuff, unsigned minChars)
    : FileTypeBase(ignorePrepStuff, minChars) {
}

ILineFilterPtr FileType_C::CreateLineFilter() const {
    return std::make_shared<CstyleCommentsLineFilter>();
}

std::string FileType_C::GetCleanLine(const std::string& line) const {
    return CstyleUtils::RemoveSingleLineComments(line);
}

bool FileType_C::IsPreprocessorDirective(const std::string& line) const {
    return line[0] == '#';
}
