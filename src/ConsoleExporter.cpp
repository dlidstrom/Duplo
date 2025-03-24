#include "ConsoleExporter.h"

#include <iostream>
#include <sstream>

ConsoleExporter::ConsoleExporter(const Options& options) {
    std::streambuf* buf;
    std::streambuf* logbuf;
    if (options.GetOutputFilename() == "-") {
        buf = std::cout.rdbuf();
        if (options.GetOutputXml() || options.GetOutputJSON()) {
            logbuf = 0;
        } else {
            logbuf = std::cout.rdbuf();
        }
    } else {
        m_of.open(options.GetOutputFilename().c_str(), std::ios::out | std::ios::binary);
        buf = m_of.rdbuf();
        logbuf = std::cout.rdbuf();
    }

    m_out = std::make_shared<std::ostream>(buf);
    m_log = std::make_shared<std::ostream>(logbuf);
    std::ostream out(buf);
    std::ostream log(logbuf);
    if (!out) {
        std::ostringstream stream;
        stream
            << "Error: Can't open file: "
            << options.GetOutputFilename()
            << std::endl;
        throw std::runtime_error(stream.str().c_str());
    }
}

void ConsoleExporter::Log(const std::string& message) {
    (*m_log) << message << std::flush;
}

void ConsoleExporter::WriteHeader() {}
void ConsoleExporter::WriteFooter(
    const Options& options,
    int files,
    long locsTotal,
    const ProcessResult& processResult) {
    (*m_out)
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
        << processResult.DuplicateLines()
        << std::endl
        << "  Total "
        << processResult.Blocks()
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
    (*m_out)
        << source1.GetFilename()
        << "(" << source1.GetLine(line1).GetLineNumber() << ")"
        << std::endl;
    (*m_out)
        << source2.GetFilename()
        << "(" << source2.GetLine(line2).GetLineNumber() << ")"
        << std::endl;
    for (int j = 0; j < count; j++) {
      (*m_out) << source1.GetLine(j + line1).GetLine() << std::endl;
    }

    (*m_out) << std::endl;
}
