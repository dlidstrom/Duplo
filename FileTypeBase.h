#ifndef _FILETYPEBASE_H_
#define _FILETYPEBASE_H_

#include "IFileType.h"
#include "ILineFilter.h"

struct FileTypeBase : public IFileType {
    bool m_ignorePrepStuff;
    unsigned m_minChars;

    FileTypeBase(bool ignorePrepStuff, unsigned minChars);

    std::vector<SourceLine> GetCleanedSourceLines(const std::vector<std::string>&) const override;

    virtual ILineFilterPtr CreateLineFilter() const = 0;

    bool IsSourceLine(const std::string& line) const override;

    virtual bool IsPreprocessorDirective(const std::string& line) const = 0;
};

#endif
