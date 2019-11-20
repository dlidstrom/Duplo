#ifndef _FILETYPE_H_2 // TODO RENAME
#define _FILETYPE_H_2

#include "FileTypeBase.h"

struct FileType_H : public FileTypeBase {
    FileType_H(bool ignorePrepStuff, unsigned minChars);

    std::vector<SourceLine> GetCleanedSourceLines(const std::vector<std::string>&) const override;

    std::string GetCleanLine(const std::string& line) const override;

    bool IsPreprocessorDirective(const std::string& line) const override;
};

#endif
