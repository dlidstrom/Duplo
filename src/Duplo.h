#ifndef _DUPLO_H_
#define _DUPLO_H_

#include "Fwd.h"

#include <string>
#include <iostream>
#include <vector>

const std::string VERSION = DUPLO_VERSION;

namespace Duplo {
    void Run(const Options& options);
}

#endif
