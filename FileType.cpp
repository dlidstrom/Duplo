#include "FileType.h"
#include "StringUtil.h"

#include <algorithm>
#include <map>

namespace {
    static const auto extensionToFileType =
        std::map<std::string, FileType::FILETYPE>{
            { "c", FileType::FILETYPE::FILETYPE_C },
            { "cpp", FileType::FILETYPE::FILETYPE_CPP },
            { "cxx", FileType::FILETYPE::FILETYPE_CXX },
            { "h", FileType::FILETYPE::FILETYPE_H },
            { "hpp", FileType::FILETYPE::FILETYPE_HPP },
            { "java", FileType::FILETYPE::FILETYPE_JAVA },
            { "cs", FileType::FILETYPE::FILETYPE_CS },
            { "vb", FileType::FILETYPE::FILETYPE_VB },
            { "s", FileType::FILETYPE::FILETYPE_S }, // gcc assembly
            { "", FileType::FILETYPE::FILETYPE_UNKNOWN },
        };
}

FileType::FILETYPE FileType::GetFileType(const std::string& FileName) {
    auto TrimFileName = StringUtil::Trim(FileName);
    auto DotPos = TrimFileName.find_last_of(".");

    std::string FileExtn = "";
    if (std::string::npos != DotPos) {
        FileExtn = TrimFileName.substr(DotPos + 1);
    }

    // get lower case file extension
    std::transform(FileExtn.begin(), FileExtn.end(), FileExtn.begin(), std::tolower);
    auto fileType = extensionToFileType.at(FileExtn);
    return fileType;
}
