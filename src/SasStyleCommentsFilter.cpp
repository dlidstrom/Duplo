#include "SasStyleCommentsFilter.h"

#include <algorithm>
#include <iostream>

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
//                std::cout << "MESSAGE COMMENT END at " << j << ": " << line << std::endl;
            } else {
                atStart = false;
            }
            continue;
        }
        if (m_openBlockComments <= 0 && atStart && line[j] == '*') {
            m_inMessageComment = true;
            atStart = false;
//            std::cout << "MESSAGE COMMENT START at " << j << ": " << line << std::endl;
            continue;
        }
        atStart = atStart && (line[j] == ' ' || line[j] == '\t');

        auto after = j == line.size() - 1 ? j : j + 1;
        if (line[j] == '/' && line[after] == '*') {
            m_openBlockComments++;
//            std::cout << "BLOCK COMMENT START at " << j << ": " << line << std::endl;
        }

        if (m_openBlockComments <= 0) {
            tmp.push_back(line[j]);
        }

        auto before = j == 0 ? 0 : j - 1;
        if (line[before] == '*' && line[j] == '/') {
            m_openBlockComments--;
//            std::cout << "BLOCK COMMENT END at " << j << ": " << line << std::endl;
        }
    }

//    if (line != tmp) {
//        std::cout << "BEFORE: " << line << std::endl;
//        std::cout << "AFTER : " << tmp << std::endl;
//    }
    return tmp;
}
