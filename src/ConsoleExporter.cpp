#include "Block.h"
#include "ConsoleExporter.h"
#include "FileExporter.h"

#include <iostream>

ConsoleExporter::ConsoleExporter(const Options& options)
    : FileExporter(options, true) {
}

void ConsoleExporter::LogMessage(const std::string& message) {
    Log() << message << std::flush;
}

void ConsoleExporter::WriteHeader() {}
void ConsoleExporter::WriteFooter(
    const Options& options,
    int files,
    long locsTotal,
    unsigned tot_dup_blocks,
    unsigned tot_dup_lines) {
    Out()
        << "Configuration:"
        << std::endl
        << "  Number of files: "
        << files
        << std::endl
        << "  Minimal block size: "
        << options.GetMinBlockSize()
        << std::endl
        << "  Minimal characters in line: "
        << options.GetMinChars()
        << std::endl
        << "  Ignore preprocessor directives: "
        << options.GetIgnorePrepStuff()
        << std::endl
        << "  Ignore same filenames: "
        << options.GetIgnoreSameFilename()
        << std::endl
        << std::endl
        << "Results:"
        << std::endl
        << "  Lines of code: "
        << locsTotal
        << std::endl
        << "  Duplicate lines of code: "
        << tot_dup_lines
        << std::endl
        << "  Total "
        << tot_dup_blocks
        << " duplicate block(s) found."
        << std::endl
        << std::endl;
}

void ConsoleExporter::ReportSeq(
    int line1,
    int line2,
    int count,
    const SourceFile& source1,
    const SourceFile& source2) {
    Out()
        << source1.GetFilename()
        << "(" << source1.GetLine(line1).GetLineNumber() << ")"
        << std::endl;
    Out()
        << source2.GetFilename()
        << "(" << source2.GetLine(line2).GetLineNumber() << ")"
        << std::endl;
    for (int j = 0; j < count; j++) {
        Out() << source1.GetLine(j + line1).GetLine() << std::endl;
    }

    Out() << std::endl;
}
