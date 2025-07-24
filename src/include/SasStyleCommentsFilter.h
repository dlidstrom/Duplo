#ifndef _SASSSTYLECOMMENTSFILTER_H_
#define _SASSSTYLECOMMENTSFILTER_H_

#include "ILineFilter.h"

/**
 * This line filter will remove all SAS style block comments.
 */
class SasStyleCommentsLineFilter : public ILineFilter {
    bool m_inMessageComment;
    int m_openBlockComments;

public:
    SasStyleCommentsLineFilter();
    std::string ProcessSourceLine(const std::string& line) override;
};

#endif
