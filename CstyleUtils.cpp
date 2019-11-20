#include "CstyleUtils.h"

std::string CstyleUtils::RemoveSingleLineComments(const std::string& line) {
    // Remove single line comments
    std::string cleanedLine;
    cleanedLine.reserve(line.size());
    auto lineSize = line.size();
    for (std::string::size_type i = 0; i < line.size(); i++) {
        if (i < lineSize - 2 && line[i] == '/' && line[i + 1] == '/') {
            return cleanedLine;
        }

        cleanedLine.push_back(line[i]);
    }

    return cleanedLine;
}
