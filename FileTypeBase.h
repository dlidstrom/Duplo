#ifndef _FILETYPEBASE_H_
#define _FILETYPEBASE_H_

#include "IFileType.h"

struct FileTypeBase : public IFileType {
    bool m_ignorePrepStuff;
    unsigned m_minChars;

    FileTypeBase(bool ignorePrepStuff, unsigned minChars);

    bool IsSourceLine(const std::string& line) const;

    virtual bool IsPreprocessorDirective(const std::string& line) const = 0;
};

#endif
