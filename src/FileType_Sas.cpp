#include "FileType_Sas.h"
#include "SasStyleCommentsFilter.h"
#include "Utils.h"

FileType_Sas::FileType_Sas(bool ignorePrepStuff, unsigned minChars)
    : FileTypeBase(ignorePrepStuff, minChars) {
}

ILineFilterPtr FileType_Sas::CreateLineFilter() const {
    return std::make_shared<SasStyleCommentsLineFilter>();
}

std::string FileType_Sas::GetCleanLine(const std::string& line) const {
    return CstyleUtils::RemoveSingleLineComments(line);
}

bool FileType_Sas::IsPreprocessorDirective(const std::string&) const {
    return false;
}
