#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "SourceFile.h"

class Block {
public:
    SourceFile const* m_source1;
    SourceFile const* m_source2;
    unsigned m_line1;
    unsigned m_line2;
    unsigned m_count;

    auto operator<=>(Block const &) const = default;
};

#endif
