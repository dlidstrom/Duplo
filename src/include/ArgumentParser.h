#ifndef _ARGUMENTPARSER_H_
#define _ARGUMENTPARSER_H_

class ArgumentParser {
private:
    int argc;
    const char** argv;

public:
    ArgumentParser(int m_argc, const char* m_argv[]);

    bool is(const char* s);
    const char* getStr(const char* s, const char* defaultValue = "");
    int getInt(const char* s, int defaultValue);
    float getFloat(const char* s, float defaultValue);
};

#endif
