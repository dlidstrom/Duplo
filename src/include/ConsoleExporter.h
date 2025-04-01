#ifndef _CONSOLEEXPORTER_H_
#define _CONSOLEEXPORTER_H_

#include "Block.h"
#include "FileExporter.h"

class ConsoleExporter : public FileExporter {
public:
    ConsoleExporter(const Options& options);
    void LogMessage(const std::string& message) override;
    void WriteHeader() override;
    void WriteFooter(
        const Options& options,
        int files,
        long locsTotal,
        unsigned tot_dup_blocks,
        unsigned tot_dup_lines) override;
    void ReportSeq(
        int line1,
        int line2,
        int count,
        const SourceFile& source1,
        const SourceFile& source2) override;
};

#endif
