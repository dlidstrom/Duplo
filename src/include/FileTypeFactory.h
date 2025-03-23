#ifndef _FILETYPEFACTORY_H_
#define _FILETYPEFACTORY_H_

#include "Fwd.h"

#include <string>

namespace FileTypeFactory {
    IFileTypePtr CreateFileType(
        const std::string& filename,
        bool ignorePrepStuff,
        unsigned minChars);
}

#endif
