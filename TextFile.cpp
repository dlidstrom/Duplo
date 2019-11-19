#include "TextFile.h"
#include "StringUtil.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace {
    std::string readAll(const std::string fileName) {
        std::string all;
        std::ifstream inFile(fileName.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
        if (!inFile) {
            std::ostringstream stream;
            stream << "Error: Can't open file: " << fileName << ". File doesn't exist or access denied.\n";
            throw std::exception(stream.str().c_str());
        }

        std::streampos len = inFile.tellg();
        inFile.seekg(0);
        all.resize(static_cast<std::size_t>(len));
        inFile.read(&all[0], len);
        return all;
    }
}

TextFile::TextFile(const std::string& fileName)
    : m_fileName(fileName) {
}

std::vector<std::string> TextFile::readLines(bool doTrim) {
    std::string list = readAll(m_fileName);

    std::vector<std::string> lines;
    StringUtil::Split(list, "\n", lines, doTrim);
    return lines;
}

void TextFile::writeAll(const std::string& all) {
    std::ofstream outFile(m_fileName.c_str(), std::ios::binary);
    if (!outFile) {
        std::ostringstream stream;
        stream << "Error: Can't open file: " << m_fileName << ". File doesn't exist or access denied.\n";
        throw std::exception(stream.str().c_str());
    }

    outFile << all;
}
