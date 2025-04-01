#include "Block.h"
#include "JsonExporter.h"
#include "FileExporter.h"

#include <iostream>

JsonExporter::JsonExporter(const Options& options)
    : FileExporter(options, false) {
}

void JsonExporter::LogMessage(const std::string& message) {
    Log() << message << std::flush;
}

void JsonExporter::WriteHeader() {
}

void JsonExporter::WriteFooter(
    const Options& /*options*/,
    int /*files*/,
    long /*locsTotal*/,
    std::vector<Block> const &/*tot_dup_blocks*/,
    unsigned /*tot_dup_lines*/) {
    Out() << m_json.dump(2, ' ', true, nlohmann::json::error_handler_t::ignore);
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
