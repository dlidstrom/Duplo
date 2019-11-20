#ifndef _FILETYPE_UNKNOWN_H_
#define _FILETYPE_UNKNOWN_H_

#include "FileTypeBase.h"

struct FileType_Unknown : public FileTypeBase {
    FileType_Unknown(unsigned minChars);

    std::vector<SourceLine> GetCleanedSourceLines(const std::vector<std::string>&) const override;

    std::string GetCleanLine(const std::string& line) const override;

    bool IsPreprocessorDirective(const std::string&) const override;
};

#endif
