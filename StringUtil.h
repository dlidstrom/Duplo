#ifndef _STRINGUTIL_H_
#define _STRINGUTIL_H_

#include <string>
#include <vector>

namespace StringUtil {
    /**
     * Trim string
     *
     * @param input  string to trim
     * @return returns trimmed string
     */
    std::string Trim(const std::string& input);

    /**
     * Split string
     *
     * @param input  string to split
     * @param delimiter  delimiter string to trim
     * @param results  results vector with substrings
     * @param trim  boolean to indicate trimming or not
     * @return returns number of substrings
     */
    int Split(const std::string& input, const std::string& delimiter, std::vector<std::string>& results, bool trim);

    std::string Substitute(char s, char d, const std::string& str);

    void StrSub(std::string& cp, const std::string& sub_this, const std::string& for_this, const int& num_times);
}

#endif
