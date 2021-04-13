#include "FileTypeFactory.h"
#include "FileType_C.h"
#include "FileType_CS.h"
#include "FileType_S.h"
#include "FileType_Unknown.h"
#include "FileType_VB.h"
#include "FileType_Ada.h"
#include "StringUtil.h"

#include <algorithm>
#include <map>

IFileTypePtr FileTypeFactory::CreateFileType(
    const std::string& filename,
    bool ignorePrepStuff,
    unsigned minChars) {
    auto ext = StringUtil::ToLower(StringUtil::GetFileExtension(filename));
    IFileTypePtr fileType;
    if (ext == "c" || ext == "cpp" || ext == "cxx" || ext == "h")
        fileType.reset(new FileType_C(ignorePrepStuff, minChars));
    else if (ext == "cs")
        fileType.reset(new FileType_CS(ignorePrepStuff, minChars));
    else if (ext == "s")
        fileType.reset(new FileType_S(ignorePrepStuff, minChars));
    else if (ext == "vb")
        fileType.reset(new FileType_VB(ignorePrepStuff, minChars));
    else if (ext == "ads" || ext == "adb")
        fileType.reset(new FileType_Ada(ignorePrepStuff, minChars));
    else
        fileType.reset(new FileType_Unknown(minChars));
    return fileType;
}
