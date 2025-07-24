#ifndef _FILETYPE_SAS_H_
#define _FILETYPE_SAS_H_

#include "FileTypeBase.h"

struct FileType_Sas : public FileTypeBase {
    FileType_Sas(bool ignorePrepStuff, unsigned minChars);

    ILineFilterPtr CreateLineFilter() const override;

    std::string GetCleanLine(const std::string& line) const override;

    bool IsPreprocessorDirective(const std::string&) const override;
};

#endif
