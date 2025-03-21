#ifndef _CSTYLECOMMENTSFILTER_H_
#define _CSTYLECOMMENTSFILTER_H_

#include "ILineFilter.h"

/**
 * This line filter will remove all C style block comments.
 */
class CstyleCommentsLineFilter : public ILineFilter {
    int m_openBlockComments;

public:
    CstyleCommentsLineFilter();
    std::string ProcessSourceLine(const std::string& line) override;
};

#endif
