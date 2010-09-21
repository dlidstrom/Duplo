#include "StringUtil.h"
#include "FileType.h"

static const std::string FileTypeExtn_C = "c";
static const std::string FileTypeExtn_CPP = "cpp";
static const std::string FileTypeExtn_CXX = "cxx";
static const std::string FileTypeExtn_H = "h";
static const std::string FileTypeExtn_HPP = "hpp";
static const std::string FileTypeExtn_Java = "java";
static const std::string FileTypeExtn_CS = "cs";
static const std::string FileTypeExtn_VB = "vb";

#include <algorithm>

FileType::FILETYPE FileType::GetFileType(const std::string& FileName)
{
    std::string TrimFileName = StringUtil::trim(FileName);
    std::string::size_type DotPos = TrimFileName.find_last_of (".");

    if (std::string::npos == DotPos)
    {
        // no file extension
        return FILETYPE_UNKNOWN;
    }

    // get lower case file extension
    std::string FileExtn = TrimFileName.substr(DotPos + 1, TrimFileName.length() - DotPos - 1);
    std::transform(FileExtn.begin(), FileExtn.end(), FileExtn.begin(), (int(*)(int)) tolower);

    if (!FileExtn.compare(FileTypeExtn_C))
    {
        return FILETYPE_C;
    }
    else if (!FileExtn.compare(FileTypeExtn_CPP))
    {
        return FILETYPE_CPP;
    }
    else if (!FileExtn.compare(FileTypeExtn_CXX))
    {
        return FILETYPE_CXX;
    }
    else if (!FileExtn.compare(FileTypeExtn_H))
    {
        return FILETYPE_H;
    }
    else if (!FileExtn.compare(FileTypeExtn_HPP))
    {
        return FILETYPE_HPP;
    }
    else if (!FileExtn.compare(FileTypeExtn_Java))
    {
        return FILETYPE_JAVA;
    }
    else if (!FileExtn.compare(FileTypeExtn_CS))
    {
        return FILETYPE_CS;
    }
    else if (!FileExtn.compare(FileTypeExtn_VB))
    {
        return FILETYPE_VB;
    }

    return FILETYPE_UNKNOWN;
}
