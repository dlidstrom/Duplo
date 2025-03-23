#ifndef _ILINEFILTER_H_
#define _ILINEFILTER_H_

#include <string>

struct ILineFilter {
    virtual ~ILineFilter() = default;
    virtual std::string ProcessSourceLine(const std::string& line) = 0;
};

typedef std::shared_ptr<ILineFilter> ILineFilterPtr;

#endif
