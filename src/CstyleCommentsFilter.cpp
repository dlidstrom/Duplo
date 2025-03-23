#include "CstyleCommentsFilter.h"

#include <algorithm>

CstyleCommentsLineFilter::CstyleCommentsLineFilter()
    : m_openBlockComments(0) {
}

std::string CstyleCommentsLineFilter::ProcessSourceLine(const std::string& line) {
    std::string tmp;
    tmp.reserve(line.size());
    for (std::string::size_type j = 0; j < line.size(); j++) {
        if (line[j] == '/' && line[std::min(line.size() - 1, j + 1)] == '*') {
            m_openBlockComments++;
        }

        if (m_openBlockComments <= 0) {
            tmp.push_back(line[j]);
        }

        auto before = j == 0 ? 0 : j - 1;
        if (line[before] == '*' && line[j] == '/') {
            m_openBlockComments--;
        }
    }

    return tmp;
}
