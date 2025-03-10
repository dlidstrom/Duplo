#ifndef _SOURCEFILE_H_
#define _SOURCEFILE_H_

#include "Fwd.h"

#include <string>
#include <vector>

class SourceFile {
    std::string m_filename;
    IFileTypePtr m_fileType;
    std::vector<SourceLine> m_sourceLines;

public:
    SourceFile(const std::string& fileName, unsigned minChars, bool ignorePrepStuff);
    SourceFile(SourceFile&& right) noexcept;

    size_t GetNumOfLines() const;
    const SourceLine& GetLine(int index) const;
    const std::string& GetFilename() const;

    bool operator==(const SourceFile& other) const;
    bool operator!=(const SourceFile& other) const;
};

#endif
