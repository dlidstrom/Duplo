/** \class StringUtil
 * StringUtil, various std::string helper methods
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

#ifndef _STRINGUTIL_H_
#define _STRINGUTIL_H_

#include <vector>
#include <string>

class StringUtil{
private:

public:
    static std::string trim(const std::string& input);
    static int split(const std::string& input, const std::string& delimiter, std::vector<std::string>& results, bool trim);
    static std::string substitute(char s, char d, const std::string& str);

    static void StrSub(std::string& cp, const std::string& sub_this, const std::string& for_this, const int& num_times);
};

#endif
