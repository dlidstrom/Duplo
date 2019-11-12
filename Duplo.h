/** \class Duplo
 * Duplo, main class
 *
 * @author  Christian Ammann (cammann@giants.ch)
 * @date  16/05/05
 *
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

#ifndef _DUPLO_H_
#define _DUPLO_H_

#include <string>
#include <iostream>

class SourceFile;

const std::string VERSION = "0.3.0";

class Duplo {
protected:
    std::string m_listFileName;
    unsigned int m_minBlockSize;
    unsigned int m_blockPercentThreshold;
    unsigned int m_minChars;
    bool m_ignorePrepStuff;
    bool m_ignoreSameFilename;
    int m_maxLinesPerFile;
    int m_DuplicateLines;
    bool m_Xml;
    unsigned char* m_pMatrix;

    void reportSeq(int line1, int line2, int count, SourceFile* pSource1, SourceFile* pSource2, std::ostream& outFile);
    int process(SourceFile* pSource1, SourceFile* pSource2, std::ostream& outFile);

    const std::string getFilenamePart(const std::string& fullpath) const;
    bool isSameFilename(const std::string& filename1, const std::string& filename2) const;

public:
    Duplo(
        const std::string& listFileName,
        unsigned int blockPercentThreshold,
        unsigned int minBlockSize,
        unsigned int minChars,
        bool ignorePrepStuff, bool ignoreSameFilename, bool Xml);
    ~Duplo();
    void run(std::string outputFileName);
};

#endif

