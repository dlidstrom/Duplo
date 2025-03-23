#ifndef _PROCESSRESULT_H_
#define _PROCESSRESULT_H_

class ProcessResult {
  unsigned m_blocks;
  unsigned m_duplicateLines;

public:
  ProcessResult();

  ProcessResult(unsigned blocks, unsigned duplicateLines);

  unsigned Blocks() const;

  unsigned DuplicateLines() const;

  friend ProcessResult operator<<(ProcessResult& left, const ProcessResult& right);
};

#endif
