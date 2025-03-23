#include "NoopLineFilter.h"

std::string NoopLineFilter::ProcessSourceLine(const std::string& line) {
    return line;
}
