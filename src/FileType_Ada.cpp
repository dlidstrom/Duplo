#include "FileType_Ada.h"
#include "Utils.h"
#include "NoopLineFilter.h"
#include "SourceLine.h"

#include <cstring>

FileType_Ada::FileType_Ada(bool ignorePrepStuff, unsigned minChars)
    : FileTypeBase(ignorePrepStuff, minChars) {
}

ILineFilterPtr FileType_Ada::CreateLineFilter() const {
    return std::make_shared<NoopLineFilter>();
}

std::string FileType_Ada::GetCleanLine(const std::string& line) const {
    std::string cleanedLine(line.substr(0, line.find("--")));
    return cleanedLine;
}

bool FileType_Ada::IsPreprocessorDirective(const std::string& line) const {
    // look for other markers to avoid
    const char* markers[] = { "pragma", "with", "use" };

    size_t first = line.find_first_not_of(" \t\r\n");
    if (first != std::string::npos)
	    first = 0;

    for (auto v : markers) {
        if (line.find(v, first, std::strlen(v)) != std::string::npos)
            return true;
    }

    return false;
}
