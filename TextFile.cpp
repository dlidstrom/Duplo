#include "TextFile.h"
#include "StringUtil.h"

#include <fstream>
#include <iostream>
#include <sstream>

/**
 * Creates a new text file. The file is accessed relative to current directory.
 */
TextFile::TextFile(const std::string& fileName) : m_fileName(fileName) {
}

/**
 * Reads the whole text file into a std::string.
 */
bool TextFile::readAll(std::string& all) {
    std::ifstream inFile(m_fileName.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
    if (inFile.is_open()) {
        std::streampos len = inFile.tellg();
        inFile.seekg(0);
        all.reserve(static_cast<std::size_t>(len));
        inFile.read(&all[0], len);
    } else {
        std::stringstream stream;
        stream << "Error: Can't open file: " << m_fileName << ". File doesn't exist or access denied.\n";
        throw std::exception(stream.str().c_str());
    }
    return true;
}

bool TextFile::readLines(std::vector<std::string>& lines, bool doTrim) {

    std::string list;
    if (readAll(list)) {
        list = StringUtil::Substitute('\r', ' ', list);
        list = StringUtil::Substitute('\t', ' ', list);
        list = StringUtil::Split(list, "\n", lines, doTrim);
        return true;
    }

    return false;
}

/**
 * Writes a std::string into a text file.
 */
bool TextFile::writeAll(const std::string& all) {

    std::ofstream outFile(m_fileName.c_str(), std::ios::binary);
    if (outFile.is_open()) {
        outFile << all;
        outFile.close();
    } else {
        std::cout << "Error: Can't open file: " << m_fileName << ". File doesn't exist or access denied.\n";
        return false;
    }

    return true;
}
