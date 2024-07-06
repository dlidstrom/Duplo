#ifndef _FILETYPE_JAVA_H_
#define _FILETYPE_JAVA_H_

#include "FileTypeBase.h"

struct FileType_Java : public FileTypeBase {
    FileType_Java(bool ignorePrepStuff, unsigned minChars);

    ILineFilterPtr CreateLineFilter() const override;

    std::string GetCleanLine(const std::string& line) const override;

    bool IsPreprocessorDirective(const std::string& line) const override;
};

#endif
