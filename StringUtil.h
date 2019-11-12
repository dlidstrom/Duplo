#ifndef _STRINGUTIL_H_
#define _STRINGUTIL_H_

#include <string>
#include <vector>

class StringUtil {
private:
public:
    static std::string trim(const std::string& input);
    static int split(const std::string& input, const std::string& delimiter, std::vector<std::string>& results, bool trim);
    static std::string substitute(char s, char d, const std::string& str);

    static void StrSub(std::string& cp, const std::string& sub_this, const std::string& for_this, const int& num_times);
};

#endif
