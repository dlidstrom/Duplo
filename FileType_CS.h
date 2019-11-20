#ifndef _FILETYPE_CS_H_
#define _FILETYPE_CS_H_

#include "FileTypeBase.h"

struct FileType_CS : public FileTypeBase {
    FileType_CS(bool ignorePrepStuff, unsigned minChars);

    std::vector<SourceLine> GetCleanedSourceLines(const std::vector<std::string>&) const override;

    std::string GetCleanLine(const std::string& line) const override;

    bool IsPreprocessorDirective(const std::string& line) const override;
};

#endif
