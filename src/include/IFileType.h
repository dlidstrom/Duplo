#ifndef _IFILETYPE_H_
#define _IFILETYPE_H_

#include "Fwd.h"

#include <string>
#include <vector>

struct IFileType {
    virtual ~IFileType() = default;
    virtual std::vector<SourceLine> GetCleanedSourceLines(const std::vector<std::string>& lines) const = 0;
};

#endif
