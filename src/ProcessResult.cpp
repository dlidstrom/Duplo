#include "ProcessResult.h"

ProcessResult::ProcessResult()
    : m_blocks(0),
      m_duplicateLines(0) {
}

ProcessResult::ProcessResult(unsigned blocks, unsigned duplicateLines)
    : m_blocks(blocks),
      m_duplicateLines(duplicateLines) {
}

unsigned ProcessResult::Blocks() const {
    return m_blocks;
}

unsigned ProcessResult::DuplicateLines() const {
    return m_duplicateLines;
}

ProcessResult operator<<(ProcessResult& left, const ProcessResult& right) {
  left.m_blocks += right.m_blocks;
  left.m_duplicateLines += right.m_duplicateLines;
  return left;
}
