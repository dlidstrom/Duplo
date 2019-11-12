#ifndef _SOURCEFILE_H_
#define _SOURCEFILE_H_

#include <string>
#include <vector>

#include "FileType.h"
#include "SourceLine.h"

#ifndef MIN
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#endif
#ifndef MAX
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#endif

class SourceFile {
protected:
    std::string m_fileName;
    FileType::FILETYPE m_FileType;

    unsigned int m_minChars;
    bool m_ignorePrepStuff;

    std::vector<SourceLine*> m_sourceLines;

    bool isSourceLine(const std::string& line);
    void getCleanLine(const std::string& line, std::string& cleanedLine);

public:
    SourceFile(const std::string& fileName, const unsigned int minChars, const bool ignorePrepStuff);

    int getNumOfLines();
    SourceLine* getLine(const int index);
    const std::string& getFilename() const;

    bool operator==(const SourceFile& other) const;
    bool operator!=(const SourceFile& other) const;
};

#endif
