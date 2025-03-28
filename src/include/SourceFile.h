#ifndef _SOURCEFILE_H_
#define _SOURCEFILE_H_

#include "IFileType.h"
#include "SourceLine.h"

#include <string>
#include <vector>

class SourceFile {
    std::string m_filename;
    IFileTypePtr m_fileType;
    std::vector<SourceLine> m_sourceLines;

public:
    SourceFile(const std::string& fileName, unsigned minChars, bool ignorePrepStuff);
    SourceFile(SourceFile&& right) noexcept;
    SourceFile &operator=(SourceFile const &other);

    size_t GetNumOfLines() const;
    const SourceLine& GetLine(int index) const;
    std::vector<std::string> GetLines(int begin, int end) const;
    const std::string& GetFilename() const;

    bool operator==(const SourceFile& other) const;
    bool operator!=(const SourceFile& other) const;
};

#endif
