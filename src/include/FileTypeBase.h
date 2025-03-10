#ifndef _FILETYPEBASE_H_
#define _FILETYPEBASE_H_

#include "IFileType.h"

class FileTypeBase : public IFileType {
    bool m_ignorePrepStuff;
    unsigned m_minChars;

    bool IsSourceLine(const std::string& line) const;

    virtual ILineFilterPtr CreateLineFilter() const = 0;
    virtual std::string GetCleanLine(const std::string& line) const = 0;
    virtual bool IsPreprocessorDirective(const std::string& line) const = 0;

public:

    FileTypeBase(bool ignorePrepStuff, unsigned minChars);
    std::vector<SourceLine> GetCleanedSourceLines(const std::vector<std::string>&) const override;
};

#endif
