/** \class ArgumentParser
 * Argument parser
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

#ifndef _ARGUMENTPARSER_H_
#define _ARGUMENTPARSER_H_

class ArgumentParser{
private:
	int argc;
	char **argv;

public:
	ArgumentParser(int m_argc, char* m_argv[]);

	bool is(char *s);
	char *getStr(char *s, char *defaultValue="");
	int getInt(char *s, int defaultValue);
	float getFloat(char *s, float defaultValue);
};

#endif
