#ifndef _IFILETYPE_H_
#define _IFILETYPE_H_

#include "Fwd.h"

#include <memory>
#include <string>
#include <vector>

struct IFileType {
    virtual std::vector<SourceLine> GetCleanedSourceLines(const std::vector<std::string>& lines) const = 0;
    virtual std::string GetCleanLine(const std::string& line) const = 0;
    virtual bool IsSourceLine(const std::string& s) const = 0;
};

typedef std::shared_ptr<IFileType> IFileTypePtr;

#endif
