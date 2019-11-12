#ifndef _FILETYPE_H_
#define _FILETYPE_H_

#include <vector>
#include <string>

class FileType
{
public:
    enum class FILETYPE
    {
        FILETYPE_UNKNOWN,
        FILETYPE_C,
        FILETYPE_CPP,
        FILETYPE_CXX,
        FILETYPE_H,
        FILETYPE_HPP,
        FILETYPE_JAVA,
        FILETYPE_CS,
        FILETYPE_VB,
        FILETYPE_S // gcc assembly
    };

public:
    static FileType::FILETYPE GetFileType(const std::string& FileName);
};

#endif

