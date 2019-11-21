#ifndef _DUPLO_H_
#define _DUPLO_H_

#include "Fwd.h"

#include <string>
#include <iostream>
#include <vector>

const std::string VERSION = "0.4.0";

namespace Duplo {
    void Run(
        unsigned minChars,
        bool ignorePrepStuff,
        unsigned minBlockSize,
        unsigned char blockPercentThreshold,
        bool xml,
        bool ignoreSameFilename,
        const std::string& listFilename,
        const std::string& outputFileName);
}

#endif
