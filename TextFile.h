#ifndef _TEXTFILE_H_
#define _TEXTFILE_H_

#include <string>
#include <vector>

class TextFile {
protected:
    std::string m_fileName;

public:
    TextFile(const std::string& fileName);
    bool readAll(std::string& all);
    bool readLines(std::vector<std::string>& lines, bool doTrim);
    bool writeAll(const std::string& all);
};

#endif
