#include "FileType.h"
#include "SourceLine.h"
#include "StringUtil.h"

#include <algorithm>
#include <map>

namespace x {
    static const auto extensionToFileType =
        std::map<std::string, FileType::FILETYPE>{
            { "c", FileType::FILETYPE::FILETYPE_C },
            { "cpp", FileType::FILETYPE::FILETYPE_CPP },
            { "cxx", FileType::FILETYPE::FILETYPE_CXX },
            { "h", FileType::FILETYPE::FILETYPE_H },
            { "hpp", FileType::FILETYPE::FILETYPE_HPP },
            { "java", FileType::FILETYPE::FILETYPE_JAVA },
            { "cs", FileType::FILETYPE::FILETYPE_CS },
            { "vb", FileType::FILETYPE::FILETYPE_VB },
            { "s", FileType::FILETYPE::FILETYPE_S }, // gcc assembly
            { "", FileType::FILETYPE::FILETYPE_UNKNOWN },
        };
}

FileType::FILETYPE FileType::GetFileType(const std::string& FileName) {
    std::string FileExtn = StringUtil::GetFileExtension(FileName);

    // get lower case file extension
    auto ext = StringUtil::ToLower(FileExtn);
    auto fileType = x::extensionToFileType.at(ext);
    return fileType;
}

namespace {
    struct FileTypeBase : public IFileType {
        bool m_ignorePrepStuff;
        unsigned m_minChars;

        FileTypeBase(bool ignorePrepStuff, unsigned minChars)
            : m_ignorePrepStuff(ignorePrepStuff),
              m_minChars(minChars) {
        }

        bool IsSourceLine(const std::string& line) const {
            std::string tmp = StringUtil::ToLower(StringUtil::Trim(line));

            // filter min size lines
            if (tmp.size() < m_minChars) {
                return false;
            }

            if (m_ignorePrepStuff && IsPreprocessorDirective(tmp))
                return false;

            // must be at least one alpha-numeric character
            bool isSourceLine =
                tmp.size() >= m_minChars && std::find_if(std::begin(tmp), std::end(tmp), isalpha) != std::end(tmp);
            return isSourceLine;
        }

        virtual bool IsPreprocessorDirective(const std::string& line) const = 0;
    };

    struct FileType_C : public FileTypeBase {
        FileType_C(bool ignorePrepStuff, unsigned minChars)
            : FileTypeBase(ignorePrepStuff, minChars) {
        }

        std::vector<SourceLine> GetCleanedSourceLines(const std::vector<std::string>&) const override {
            return std::vector<SourceLine>();
        }

        bool IsPreprocessorDirective(const std::string& line) const override {
            return line[0] == '#';
        }
    };

    struct FileType_CS : public FileTypeBase {
        FileType_CS(bool ignorePrepStuff, unsigned minChars)
            : FileTypeBase(ignorePrepStuff, minChars) {
        }

        std::vector<SourceLine> GetCleanedSourceLines(const std::vector<std::string>&) const override {
            return std::vector<SourceLine>();
        }

        bool IsPreprocessorDirective(const std::string& line) const override {
            if (line[0] == '#')
                return true;

            // look for attribute
            if (line[0] == '[') {
                return true;
            }

            // look for other markers to avoid
            const std::string PreProc_CS[] = { "using", "private", "protected", "public" };

            for (int i = 0; i < 4; i++) {
                if (line.find(PreProc_CS[i].c_str(), 0, PreProc_CS[i].length()) != std::string::npos)
                    return true;
            }

            return false;
        }
    };

    struct FileType_H : public FileTypeBase {
        FileType_H(bool ignorePrepStuff, unsigned minChars)
            : FileTypeBase(ignorePrepStuff, minChars) {
        }

        std::vector<SourceLine> GetCleanedSourceLines(const std::vector<std::string>&) const override {
            return std::vector<SourceLine>();
        }

        bool IsPreprocessorDirective(const std::string& line) const override {
            return line[0] == '#';
        }
    };

    struct FileType_Unknown : public FileTypeBase {
        FileType_Unknown(unsigned minChars)
            : FileTypeBase(false, minChars) {
        }

        std::vector<SourceLine> GetCleanedSourceLines(const std::vector<std::string>&) const override {
            return std::vector<SourceLine>();
        }

        bool IsPreprocessorDirective(const std::string&) const override {
            return false;
        }
    };
}

IFileTypePtr FileTypeFactory::CreateFileType(
    const std::string& filename,
    bool ignorePrepStuff,
    unsigned minChars) {
    auto ext = StringUtil::ToLower(StringUtil::GetFileExtension(filename));
    IFileTypePtr fileType;
    if (ext == "c")
        fileType.reset(new FileType_C(ignorePrepStuff, minChars));
    else if (ext == "cs")
        fileType.reset(new FileType_CS(ignorePrepStuff, minChars));
    else if (ext == "h")
        fileType.reset(new FileType_H(ignorePrepStuff, minChars));
    else
        fileType.reset(new FileType_Unknown(minChars));
    return fileType;
}
