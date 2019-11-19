#ifndef _TEXTFILE_H_
#define _TEXTFILE_H_

#include <string>
#include <vector>

class TextFile {
    std::string m_fileName;

public:
    /**
     * Creates a new text file. The file is accessed relative to current directory.
     */
    TextFile(const std::string& fileName);

    std::vector<std::string> readLines(bool doTrim);

    /**
     * Writes a std::string into a text file.
     */
    void writeAll(const std::string& all);
};

#endif
