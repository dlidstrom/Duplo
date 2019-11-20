#ifndef _FILETYPE_C_H
#define _FILETYPE_C_H_

#include "FileTypeBase.h"

struct FileType_C : public FileTypeBase {
    FileType_C(bool ignorePrepStuff, unsigned minChars);

    std::vector<SourceLine> GetCleanedSourceLines(const std::vector<std::string>&) const override;

    std::string GetCleanLine(const std::string& line) const override;

    bool IsPreprocessorDirective(const std::string& line) const override;
};

#endif
