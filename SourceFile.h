#ifndef _SOURCEFILE_H_
#define _SOURCEFILE_H_

#include <string>
#include <vector>

#include "FileType.h"
#include "SourceLine.h"

class SourceFile {
    std::string m_fileName;
    FileType::FILETYPE m_FileType;
    IFileTypePtr m_fileType;

    unsigned int m_minChars;
    bool m_ignorePrepStuff;

    std::vector<SourceLine> m_sourceLines;

    bool isSourceLine(const std::string& line) const;
    void getCleanLine(const std::string& line, std::string& cleanedLine);

public:
    SourceFile(const std::string& fileName, unsigned int minChars, bool ignorePrepStuff);
    SourceFile(SourceFile&& right) noexcept;

    unsigned getNumOfLines() const;
    const SourceLine& getLine(int index) const;
    const std::string& getFilename() const;

    bool operator==(const SourceFile& other) const;
    bool operator!=(const SourceFile& other) const;
};

#endif
