#ifndef _FILETYPE_VB_H_
#define _FILETYPE_VB_H_

#include "FileTypeBase.h"

struct FileType_VB : public FileTypeBase {
    FileType_VB(bool ignorePrepStuff, unsigned minChars);

    ILineFilterPtr CreateLineFilter() const override;

    std::string GetCleanLine(const std::string& line) const override;

    bool IsPreprocessorDirective(const std::string& line) const override;
};

#endif
