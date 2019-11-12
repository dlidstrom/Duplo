/** \class SourceLine
 * One line of source
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

#ifndef _SOURCELINE_H_
#define _SOURCELINE_H_

#include <string>
#include <vector>

class SourceLine {
protected:
    std::string m_line;
    int m_lineNumber;

public:
    SourceLine(std::string& line, int lineNumber);

    long long m_hashHigh;
    long long m_hashLow;

    int getLineNumber();
    std::string& getLine();
    bool equals(SourceLine* pLine);
};

#endif
