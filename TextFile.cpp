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
        unsigned int len = inFile.tellg();
        inFile.seekg(0, std::ios::beg);
        char* buffer = new char[len];
        inFile.read(buffer, len);
        inFile.close();
        std::ostringstream os;
        os.write(buffer, len);
        delete[] buffer;
        all = os.str();
    } else {
        std::cout << "Error: Can't open file: " << m_fileName << ". File doesn't exist or access denied.\n";
        return false;
    }
    return true;
}

bool TextFile::readLines(std::vector<std::string>& lines, bool doTrim) {

    std::string list;
    if (readAll(list)) {
        StringUtil::substitute('\r', ' ', list);
        StringUtil::substitute('\t', ' ', list);
        StringUtil::split(list, "\n", lines, doTrim);
    } else {
        return false;
    }
    return true;
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
