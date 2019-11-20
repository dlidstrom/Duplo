#ifndef _SOURCEFILE_H_
#define _SOURCEFILE_H_

#include <string>
#include <vector>

#include "FileType.h"
#include "SourceLine.h"

class SourceFile {
    std::string m_filename;
    IFileTypePtr m_fileType;
    std::vector<SourceLine> m_sourceLines;

public:
    SourceFile(const std::string& fileName, unsigned int minChars, bool ignorePrepStuff);
    SourceFile(SourceFile&& right) noexcept;

    unsigned GetNumOfLines() const;
    const SourceLine& GetLine(int index) const;
    const std::string& GetFilename() const;

    bool operator==(const SourceFile& other) const;
};

#endif
