#include "SourceFile.h"
#include "FileTypeFactory.h"
#include "IFileType.h"
#include "SourceLine.h"
#include "StringUtil.h"
#include "TextFile.h"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <vector>

SourceFile::SourceFile(const std::string& filename, unsigned minChars, bool ignorePrepStuff)
    : m_filename(filename),
      m_fileType(FileTypeFactory::CreateFileType(filename, ignorePrepStuff, minChars)) {
    TextFile listOfFiles(m_filename);

    auto lines = listOfFiles.ReadLines(false);
    m_sourceLines = m_fileType->GetCleanedSourceLines(lines);
}

SourceFile::SourceFile(SourceFile&& right) noexcept
    : m_filename(std::move(right.m_filename))
    , m_fileType(std::move(right.m_fileType))
    , m_sourceLines(std::move(right.m_sourceLines)) {
}

size_t SourceFile::GetNumOfLines() const {
    return m_sourceLines.size();
}

const SourceLine& SourceFile::GetLine(int index) const {
    return m_sourceLines[index];
}

std::vector<std::string> SourceFile::GetLines(int begin, int end) const {
    std::vector<std::string> lines;
    lines.reserve(end - begin);
    auto begin_it = std::next(m_sourceLines.begin(), begin);
    auto end_it = std::next(m_sourceLines.begin(), end);
    std::transform(begin_it, end_it, std::back_inserter(lines),
                   [](SourceLine const& line) {
                       return line.GetLine();
                   }
    );
    return lines;
}

const std::string& SourceFile::GetFilename() const {
    return m_filename;
}

bool SourceFile::operator==(const SourceFile& other) const {
    return this == &other || GetFilename() == other.GetFilename();
}

bool SourceFile::operator!=(const SourceFile& other) const {
    return !(*this == other);
}
