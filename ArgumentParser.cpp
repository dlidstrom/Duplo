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

ArgumentParser::ArgumentParser(int m_argc, const char* m_argv[]) {
    argc = m_argc;
    argv = m_argv;
}

bool ArgumentParser::is(const char* s) {
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], s) == 0) {
            return true;
        }
    }
    return false;
}

const char* ArgumentParser::getStr(const char* s, const char* defaultValue) {
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], s) == 0 && argc > i + 1) {
            return argv[i + 1];
        }
    }

    return defaultValue;
}

int ArgumentParser::getInt(const char* s, int defaultValue) {
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], s) == 0 && argc > i + 1) {
            return atoi(argv[i + 1]);
        }
    }

    return defaultValue;
}

float ArgumentParser::getFloat(const char* s, float defaultValue) {
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], s) == 0 && argc > i + 1) {
            return (float)atof(argv[i + 1]);
        }
    }

    return defaultValue;
}
