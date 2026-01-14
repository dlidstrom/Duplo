#ifndef _FILETYPE_ERLANG_H_
#define _FILETYPE_ERLANG_H_

#include "FileTypeBase.h"

struct FileType_Erlang : public FileTypeBase {
    FileType_Erlang(bool ignorePrepStuff, unsigned minChars);

    ILineFilterPtr CreateLineFilter() const override;

    std::string GetCleanLine(const std::string& line) const override;

    bool IsPreprocessorDirective(const std::string& line) const override;
};

#endif
