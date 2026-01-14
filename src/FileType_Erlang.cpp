#include "FileType_Erlang.h"
#include "NoopLineFilter.h"

#include <cstring>

FileType_Erlang::FileType_Erlang(bool ignorePrepStuff, unsigned minChars)
    : FileTypeBase(ignorePrepStuff, minChars) {
}

ILineFilterPtr FileType_Erlang::CreateLineFilter() const {
    return std::make_shared<NoopLineFilter>();
}

std::string FileType_Erlang::GetCleanLine(const std::string& line) const {
    // Erlang uses % for single-line comments
    std::string cleanedLine(line.substr(0, line.find('%')));
    return cleanedLine;
}

bool FileType_Erlang::IsPreprocessorDirective(const std::string& line) const {
    // Erlang module attributes that start with '-'
    const char* markers[] = {
        "-module", "-export", "-import", "-include", "-include_lib",
        "-define", "-ifdef", "-ifndef", "-else", "-endif", "-undef",
        "-spec", "-type", "-opaque", "-callback", "-record",
        "-behaviour", "-behavior", "-compile", "-vsn", "-on_load", "-nifs"
    };

    size_t first = line.find_first_not_of(" \t\r\n");
    if (first == std::string::npos)
        return false;

    for (auto v : markers) {
        if (line.find(v, first, std::strlen(v)) == first)
            return true;
    }

    return false;
}
