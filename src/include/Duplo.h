#ifndef _DUPLO_H_
#define _DUPLO_H_

#include "Fwd.h"

#include <string>
#include <iostream>
#include <vector>

#ifdef DUPLO_VERSION
const std::string VERSION = DUPLO_VERSION;
#else
const std::string VERSION = "";
#endif

namespace Duplo {
    int Run(const Options& options);
}

#endif
