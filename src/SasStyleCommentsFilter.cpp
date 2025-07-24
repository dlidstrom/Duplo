#include "SasStyleCommentsFilter.h"

#include <algorithm>

SasStyleCommentsLineFilter::SasStyleCommentsLineFilter()
    : m_inMessageComment(false),
      m_openBlockComments(0)
{
}

std::string SasStyleCommentsLineFilter::ProcessSourceLine(const std::string& line) {
    std::string tmp;
    bool atStart = true;
    tmp.reserve(line.size());
    for (std::string::size_type j = 0; j < line.size(); j++) {
        if (m_inMessageComment) {
            if (line[j] == ';') {
                m_inMessageComment = false;
                atStart = true;
            } else {
                atStart = false;
            }
            continue;
        }
        if (atStart && line[j] == '*') {
            m_inMessageComment = true;
            atStart = false;
            continue;
        }
        atStart = atStart && (line[j] == ' ' || line[j] == '\t');

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
