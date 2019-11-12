/**
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <fstream>
#include <iostream>
#include <sstream>

#include "TextFile.h"

#include "StringUtil.h"

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
