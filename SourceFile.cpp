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

#include "SourceFile.h"

#include "TextFile.h"
#include "StringUtil.h"

#include <algorithm>
#include <assert.h>

SourceFile::SourceFile(const std::string& fileName, const unsigned int minChars, const bool ignorePrepStuff) :
    m_fileName(fileName),
    m_FileType(FileType::GetFileType(fileName)),
    m_minChars(minChars),    
    m_ignorePrepStuff(ignorePrepStuff)
{
    TextFile listOfFiles(m_fileName.c_str());

    std::vector<std::string> lines;
    listOfFiles.readLines(lines, false);
    
    int openBlockComments = 0;
    for(int i=0;i<(int)lines.size();i++){
        std::string& line = lines[i];
        std::string tmp;

        tmp.reserve(line.size());

        // Remove block comments
        if (FileType::FILETYPE_C    == m_FileType ||
            FileType::FILETYPE_CPP  == m_FileType ||
            FileType::FILETYPE_CXX  == m_FileType ||
            FileType::FILETYPE_H    == m_FileType ||
            FileType::FILETYPE_HPP  == m_FileType ||
            FileType::FILETYPE_JAVA == m_FileType ||
            FileType::FILETYPE_CS   == m_FileType ){
            int lineSize = (int)line.size();
            for(int j=0;j<(int)line.size();j++){
                if(line[j] == '/' && line[MIN(lineSize-1, j+1)] == '*'){
                    openBlockComments++;
                }

                if(openBlockComments <= 0){
                    tmp.push_back(line[j]);
                }

                if(line[MAX(0, j-1)] == '*' && line[j] == '/'){
                    openBlockComments--;
                }
            }
        }
        if (FileType::FILETYPE_VB == m_FileType || FileType::FILETYPE_UNKNOWN  == m_FileType) {
            tmp = line;
        }

        std::string cleaned;
        getCleanLine(tmp, cleaned);
        
        if(isSourceLine(cleaned)){
            m_sourceLines.push_back(new SourceLine(cleaned, i));
        }
    }
}

void SourceFile::getCleanLine(const std::string& line, std::string& cleanedLine){
    // Remove single line comments
    cleanedLine.reserve(line.size());
    int lineSize = (int)line.size();
    for(int i=0;i<(int)line.size();i++){
        switch (m_FileType)
        {
            case FileType::FILETYPE_C   :
            case FileType::FILETYPE_CPP :
            case FileType::FILETYPE_CXX :
            case FileType::FILETYPE_H   :
            case FileType::FILETYPE_HPP :
            case FileType::FILETYPE_JAVA:
            case FileType::FILETYPE_CS  :
                if(i < lineSize-2 && line[i] == '/' && line[i+1] == '/'){
                    return;
                }
                break;

            case FileType::FILETYPE_VB  :
                if(i < lineSize-1 && line[i] == '\''){
                    return;
                }
                break;
            
            // no pre-processing of code of unknown languages
            case FileType::FILETYPE_UNKNOWN:
                break;
        }
        cleanedLine.push_back(line[i]);
    }
}

bool SourceFile::isSourceLine(const std::string& line){
    std::string tmp = StringUtil::trim(line);

    // filter min size lines
    if (tmp.size() < m_minChars)
    {
        return false;
    }

    std::transform(tmp.begin(), tmp.end(), tmp.begin(), (int(*)(int)) tolower);

    if(m_ignorePrepStuff){
       switch (m_FileType)
       {
       case FileType::FILETYPE_C   :
       case FileType::FILETYPE_CPP :
       case FileType::FILETYPE_CXX :
       case FileType::FILETYPE_H   :
       case FileType::FILETYPE_HPP :
       case FileType::FILETYPE_JAVA:
          if(tmp[0] == '#')
          {
             return false;
          }
          break;

       case FileType::FILETYPE_CS  :
          {
             if(tmp[0] == '#')
             {
                return false;
             }
             // look for attribute
             if(tmp[0] == '[')
             {
                return false;
             }
             // look for other markers to avoid
             const std::string PreProc_CS[] = { "using", "private", "protected", "public" };

             for (int i=0; i<4; i++ ) {
                if (tmp.find(PreProc_CS[i].c_str(), 0, PreProc_CS[i].length())!=std::string::npos)
                   return false;
             }
          }
          break;

       case FileType::FILETYPE_VB  :
          {
             // look for preprocessor marker in start of string
             const std::string PreProc_VB = "imports";

             return std::string::npos == tmp.find(PreProc_VB.c_str(), 0, PreProc_VB.length());
          }
          break;
          
        // no pre-processing of code of unknown languages
        case FileType::FILETYPE_UNKNOWN:
            break;
          
       }
    }

    bool bRet = ((int)tmp.size() >= m_minChars);
    assert(bRet);
    
    // must be at least one alpha-numeric character
    return bRet && std::find_if(tmp.begin(), tmp.end(), isalpha)!=tmp.end();
}

int SourceFile::getNumOfLines(){
    return (int)m_sourceLines.size();
}

SourceLine* SourceFile::getLine(const int index){
    return m_sourceLines[index];
}

const std::string& SourceFile::getFilename() const {
    return m_fileName;
}

bool SourceFile::operator==(const SourceFile &other) const {
    return (this == &other) || (getFilename() == other.getFilename());
}

bool SourceFile::operator!=(const SourceFile &other) const {
    return !(*this == other);
}
