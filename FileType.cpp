#include "FileType.h"
#include "FileType_C.h"
#include "FileType_CS.h"
#include "FileType_H.h"
#include "FileType_S.h"
#include "FileType_Unknown.h"
#include "FileType_VB.h"
#include "SourceLine.h"
#include "StringUtil.h"

#include <algorithm>
#include <map>

namespace x {
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
    std::string FileExtn = StringUtil::GetFileExtension(FileName);

    // get lower case file extension
    auto ext = StringUtil::ToLower(FileExtn);
    auto fileType = x::extensionToFileType.at(ext);
    return fileType;
}

IFileTypePtr FileTypeFactory::CreateFileType(
    const std::string& filename,
    bool ignorePrepStuff,
    unsigned minChars) {
    auto ext = StringUtil::ToLower(StringUtil::GetFileExtension(filename));
    IFileTypePtr fileType;
    if (ext == "c")
        fileType.reset(new FileType_C(ignorePrepStuff, minChars));
    else if (ext == "cs")
        fileType.reset(new FileType_CS(ignorePrepStuff, minChars));
    else if (ext == "h")
        fileType.reset(new FileType_H(ignorePrepStuff, minChars));
    else if (ext == "s")
        fileType.reset(new FileType_S(ignorePrepStuff, minChars));
    else if (ext == "vb")
        fileType.reset(new FileType_VB(ignorePrepStuff, minChars));
    else
        fileType.reset(new FileType_Unknown(minChars));
    return fileType;
}
