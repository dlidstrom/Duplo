#include "JsonExporter.h"

#include <iostream>
#include <sstream>

JsonExporter::JsonExporter(const Options& options) {
    std::streambuf* buf;
    std::streambuf* logbuf;
    if (options.GetOutputFilename() == "-") {
        buf = std::cout.rdbuf();
        logbuf = 0;
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

void JsonExporter::Log(const std::string& message) {
    (*m_log) << message << std::flush;
}

void JsonExporter::WriteHeader() {
}

void JsonExporter::WriteFooter(
    const Options& options,
    int files,
    long locsTotal,
    const ProcessResult& processResult) {
    (*m_out) << m_json.dump(2);
}

void JsonExporter::ReportSeq(
    int line1,
    int line2,
    int count,
    const SourceFile& source1,
    const SourceFile& source2) {
    int begin = line1;
    int end = line1 + count;
    int src_begin1 = source1.GetLine(line1).GetLineNumber();
    int src_end1 = source1.GetLine(line1 + count).GetLineNumber();
    int src_begin2 = source2.GetLine(line2).GetLineNumber();
    int src_end2 = source2.GetLine(line2 + count).GetLineNumber();
    m_json.emplace_back(nlohmann::json{
        { "LineCount", end - begin },
        { "SourceFile1", source1.GetFilename() },
        { "StartLineNumber1", src_begin1 },
        { "EndLineNumber1", src_end1 },
        { "SourceFile2", source2.GetFilename() },
        { "StartLineNumber2", src_begin2 },
        { "EndLineNumber2", src_end2 },
        { "Lines", source1.GetLines(begin, end) } });
}
