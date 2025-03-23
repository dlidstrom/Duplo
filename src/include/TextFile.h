#ifndef _TEXTFILE_H_
#define _TEXTFILE_H_

#include <string>
#include <vector>

class TextFile {
    std::string m_filename;

public:
    /**
     * Creates a new text file. The file is accessed relative to current directory.
     */
    TextFile(const std::string& filename);

    std::vector<std::string> ReadLines(bool doTrim);

    /**
     * Writes a std::string into a text file.
     */
    void WriteAll(const std::string& all);
};

#endif
