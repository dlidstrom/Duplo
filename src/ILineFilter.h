#ifndef _ILINEFILTER_H_
#define _ILINEFILTER_H_

#include <memory>
#include <string>

struct ILineFilter {
    virtual ~ILineFilter() {}
    virtual std::string ProcessSourceLine(const std::string& line) = 0;
};

#endif
