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

    Block(SourceFile const* source1,
          SourceFile const* source2,
          unsigned line1,
          unsigned line2,
          unsigned count)
          : m_source1{ source1 },
            m_source2{ source2 },
            m_line1{ line1 },
            m_line2{ line2 },
            m_count{ count } {}
};

#endif
