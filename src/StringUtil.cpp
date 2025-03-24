#include "Utils.h"

#include <algorithm>
#include <locale>

namespace {
    auto& facet = std::use_facet<std::ctype<char>>(std::locale());
}

std::string StringUtil::Trim(const std::string& input) {
    // If string is empty, there is nothing to look for.
    if (input.length() == 0) {
        return "";
    }

    // Set up temporary
    std::string final = input;

    // Remove spaces at beginning
    decltype(input.length()) i = 0;
    while (i < input.length() && input[i] <= ' ') {
        i++;
    }

    // String full of spaces, return nothing.
    if (i >= input.length()) {
        return "";
    }

    if (i > 0) {
        final = input.substr(i, input.length() - i);
    }

    // Remove spaces at end
    i = final.length() - 1;
    while (i >= 0 && final[i] <= ' ') {
        i--;
    }

    final = final.substr(0, i + 1);

    // Return the new string
    return final;
}

int StringUtil::Split(const std::string& input, const std::string& delimiter, std::vector<std::string>& results, bool doTrim) {
    auto sizeDelim = delimiter.size();
    auto newPos = input.find(delimiter, 0);

    if (newPos == std::string::npos) {
        if (doTrim) {
            results.push_back(Trim(input));
        } else {
            results.push_back(input);
        }

        return 0;
    }

    // At the beginning is always a marker
    std::vector<int> positions(1, -static_cast<int>(sizeDelim));
    auto pos = input.find(delimiter);
    while (pos != std::string::npos) {
        positions.push_back(pos);
        pos = input.find(delimiter, pos + sizeDelim);
    }

    // At the end is always a marker
    positions.push_back(input.size());

    for (auto i = 0U; i < positions.size() - 1; i++) {
        std::string s;

        auto start = positions[i] + sizeDelim;
        auto size = positions[i + 1] - positions[i] - sizeDelim;

        if (size > 0) {
            s = input.substr(start, size);
        }

        if (doTrim) {
            results.push_back(Trim(s));
        } else {
            results.push_back(s);
        }
    }

    return positions.size() - 2;
}

std::string StringUtil::Substitute(char s, char d, const std::string& str) {
    std::string tmp(str);

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

std::string StringUtil::ToLower(const std::string& s) {
    std::string copy;
    copy.reserve(s.length());
    std::transform(
        std::begin(s),
        std::end(s),
        std::back_inserter(copy),
        [](char c) -> char { return facet.tolower(c); });
    return copy;
}

std::string StringUtil::GetFileExtension(const std::string& filename) {
    auto trimFileName = Trim(filename);
    auto DotPos = trimFileName.find_last_of('.');

    std::string ext;
    if (std::string::npos != DotPos) {
        ext = trimFileName.substr(DotPos + 1);
    }

    return ext;
}

std::string StringUtil::GetFilenamePart(const std::string& fullpath) {
    std::string path = StringUtil::Substitute('\\', '/', fullpath);

    std::string filename = path;

    std::string::size_type idx = path.rfind('/');
    if (idx != std::string::npos) {
        filename = path.substr(idx + 1, path.size() - idx - 1);
    }

    return filename;
}

bool StringUtil::IsSameFilename(const SourceFile& left, const SourceFile& right) {
    return StringUtil::GetFilenamePart(left.GetFilename()) == StringUtil::GetFilenamePart(right.GetFilename());
}
