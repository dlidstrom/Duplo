#include "HashUtil.h"

// https://softwareengineering.stackexchange.com/a/49599/30671
unsigned long HashUtil::Hash(const char* dataToHash, unsigned long length) {
    unsigned long int h = 2166136261UL;
    unsigned long int i;

    for (i = 0; i < length; i++)
        h = (h ^ dataToHash[i]) * 16777619;

    return h;
}
