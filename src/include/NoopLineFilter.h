#ifndef _NOOPLINEFILTER_H_
#define _NOOPLINEFILTER_H_

#include "ILineFilter.h"

struct NoopLineFilter : public ILineFilter {
    std::string ProcessSourceLine(const std::string& line) override;
};

#endif
