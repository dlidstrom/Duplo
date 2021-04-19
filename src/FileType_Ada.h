#ifndef _FILETYPE_ADA_H_
#define _FILETYPE_ADA_H_

#include "FileTypeBase.h"

struct FileType_Ada : public FileTypeBase {
    FileType_Ada(bool ignorePrepStuff, unsigned minChars);

    ILineFilterPtr CreateLineFilter() const override;

    std::string GetCleanLine(const std::string& line) const override;

    bool IsPreprocessorDirective(const std::string& line) const override;
};

#endif
