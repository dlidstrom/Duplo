#ifndef _FILETYPE_S_H_
#define _FILETYPE_S_H_

#include "FileTypeBase.h"

struct FileType_S : public FileTypeBase {
    FileType_S(bool ignorePrepStuff, unsigned minChars);

    ILineFilterPtr CreateLineFilter() const override;

    std::string GetCleanLine(const std::string& line) const override;

    bool IsPreprocessorDirective(const std::string& line) const override;
};

#endif
