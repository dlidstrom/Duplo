#ifndef _IFILETYPE_H_
#define _IFILETYPE_H_

#include "SourceLine.h"

#include <string>
#include <vector>
#include <memory>

struct IFileType {
    virtual ~IFileType() = default;
    virtual std::vector<SourceLine> GetCleanedSourceLines(const std::vector<std::string>& lines) const = 0;
};

typedef std::shared_ptr<IFileType> IFileTypePtr;

#endif
