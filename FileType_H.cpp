#include "FileType_H.h"
#include "CstyleCommentsFilter.h"
#include "CstyleUtils.h"
#include "SourceLine.h"

FileType_H::FileType_H(bool ignorePrepStuff, unsigned minChars)
    : FileTypeBase(ignorePrepStuff, minChars) {
}

ILineFilterPtr FileType_H::CreateLineFilter() const {
    return std::make_shared<CstyleCommentsLineFilter>();
}

std::string FileType_H::GetCleanLine(const std::string& line) const {
    return CstyleUtils::RemoveSingleLineComments(line);
}

bool FileType_H::IsPreprocessorDirective(const std::string& line) const {
    return line[0] == '#';
}
