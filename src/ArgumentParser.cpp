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
