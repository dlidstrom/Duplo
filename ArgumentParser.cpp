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

#include "ArgumentParser.h"

#include <cstdlib>
#include <cstring>

#ifdef _WIN32
    #include <io.h>
#endif

ArgumentParser::ArgumentParser(int m_argc, const char* m_argv[]){
    argc = m_argc;
    argv = m_argv;
}

bool ArgumentParser::is(const char *s){
    for(int i=0;i<argc;i++){
        if(strcmp(argv[i], s) == 0){
            return true;
        }
    }
    return false;
}

const char *ArgumentParser::getStr(const char *s, const char *defaultValue){
    for(int i=0;i<argc;i++){
        if(strcmp(argv[i], s) == 0 && argc > i+1){
            return argv[i+1];
        }
    }
    return defaultValue;
}

int ArgumentParser::getInt(const char *s, int defaultValue){
    for(int i=0;i<argc;i++){
        if(strcmp(argv[i], s) == 0 && argc > i+1){
            return atoi(argv[i+1]);
        }
    }

    return defaultValue;
}

void ArgumentParser::winGlob( Duplo & duplo, const std::string & path ) const
{
	intptr_t handle;
	struct _finddata_t fileinfo;

	handle = _findfirst(path.c_str(), &fileinfo);
	if (handle == -1) {
		std::cerr << "Error opening " << path << std::endl;
		return ;
	}
	duplo.pushFileName( fileinfo.name );
	while ( _findnext(handle, &fileinfo) == 0 )
		duplo.pushFileName(fileinfo.name);
	_findclose( handle );
}

void ArgumentParser::getFileNames( Duplo & duplo ) const {
    for ( int i = 1; i < argc - 1; i++ ) {
		if (argv[i][0] != '-') {
#ifdef _WIN32
			if (std::string(argv[i]).find("*") != std::string::npos) {
				winGlob(duplo, argv[i]);
				continue;
			}
#endif
			std::ifstream f(argv[i]);
			if (f.good())
				duplo.pushFileName(argv[i]);
			f.close();
		}
    }
}

float ArgumentParser::getFloat(const char *s, float defaultValue){
    for(int i=0;i<argc;i++){
        if(strcmp(argv[i], s) == 0 && argc > i+1){
            return (float)atof(argv[i+1]);
        }
    }

    return defaultValue;
}
