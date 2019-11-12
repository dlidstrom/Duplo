#include "StringUtil.h"

#include <ctype.h>

/**
  * Trim string
  *
  * @param input  string to trim
  * @return returns trimmed string
  */
std::string StringUtil::trim(const std::string& input) {
    // If string is empty, there is nothing to look for.
    if (input.length() == 0) {
        return "";
    }

    // Set up temporary
    std::string final = input;

    // Remove spaces at beginning
    int i = 0;
    while (i < (int)input.length() && input[i] <= ' ') {
        i++;
    }

    // String full of spaces, return nothing.
    if (i >= (int)input.length()) {
        return "";
    }

    if (i > 0) {
        final = input.substr(i, input.length() - i);
    }

    // Remove spaces at end
    i = (int)final.length() - 1;
    while (i >= 0 && final[i] <= ' ') {
        i--;
    }

    final = final.substr(0, i + 1);

    // Return the new string
    return final;
}

/**
 * Split string
 *
 * @param input  string to split
 * @param delimiter  delimiter string to trim
 * @param results  results vector with substrings
 * @param trim  boolean to indicate trimming or not
 * @return returns number of substrings
 */
int StringUtil::split(const std::string& input, const std::string& delimiter, std::vector<std::string>& results, bool doTrim) {
    int sizeDelim = (int)delimiter.size();

    int newPos = (int)input.find(delimiter, 0);

    if (newPos < 0) {
        if (doTrim) {
            results.push_back(trim(input));
        } else {
            results.push_back(input);
        }
        return 0;
    }

    int numFound = 0;

    std::vector<int> positions;

    // At the begin is always a marker
    positions.push_back(-sizeDelim);
    int pos = 0;
    while (pos != -1) {
        numFound++;
        pos = (int)input.find(delimiter, pos + sizeDelim);
        if (pos != -1) {
            positions.push_back(pos);
        }
    }

    // At the end is always a marker
    positions.push_back((int)input.size());

    for (int i = 0; i < (int)positions.size() - 1; i++) {
        std::string s;

        int start = positions[i] + sizeDelim;
        int size = positions[i + 1] - positions[i] - sizeDelim;

        if (size > 0) {
            s = input.substr(start, size);
        }

        if (doTrim) {
            results.push_back(trim(s));
        } else {
            results.push_back(s);
        }
    }

    return numFound;
}

std::string StringUtil::substitute(char s, char d, const std::string& str) {
    std::string tmp = str;

    for (int i = 0; i < (int)tmp.size(); i++) {
        if (tmp[i] == s) {
            tmp[i] = d;
        }
    }

    return tmp;
}

// from:
//      http://www.codecomments.com/archive272-2005-4-473566.html
void StringUtil::StrSub(std::string& cp, const std::string& sub_this, const std::string& for_this, const int& num_times) {
    int loc = 0;
    if (cp.empty()) {
        cp = sub_this;
        return;
    }
    for (int i = 0; i != num_times; i++) {
        loc = (int)cp.find(for_this, loc);
        if (loc >= 0) {
            cp.replace(loc, for_this.length(), sub_this);
            loc += (int)for_this.length();
        } else {
            return;
        }
    }
}
