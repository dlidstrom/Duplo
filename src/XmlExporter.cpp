#include "XmlExporter.h"
#include "Utils.h"

#include <iostream>
#include <sstream>

XmlExporter::XmlExporter(const Options& options) {
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

void XmlExporter::Log(const std::string& message) {
    (*m_log) << message << std::flush;
}

void XmlExporter::WriteHeader() {
    (*m_out)
        << "<?xml version=\"1.0\"?>"
        << std::endl
        << "<duplo>"
        << std::endl;
}

void XmlExporter::WriteFooter(
    const Options& options,
    int files,
    long locsTotal,
    const ProcessResult& processResult) {
    (*m_out)
        << "</duplo>"
        << std::endl;
}

void XmlExporter::ReportSeq(
    int line1,
    int line2,
    int count,
    const SourceFile& source1,
    const SourceFile& source2) {
    (*m_out)
        << "    <set LineCount=\"" << count << "\">"
        << std::endl;
    int startLineNumber1 = source1.GetLine(line1).GetLineNumber();
    int endLineNumber1 = source1.GetLine(line1 + count).GetLineNumber();
    (*m_out)
        << "        <block SourceFile=\"" << source1.GetFilename()
        << "\" StartLineNumber=\"" << startLineNumber1
        << "\" EndLineNumber=\"" << endLineNumber1 << "\"/>"
        << std::endl;
    int startLineNumber2 = source2.GetLine(line2).GetLineNumber();
    int endLineNumber2 = source2.GetLine(line2 + count).GetLineNumber();
    (*m_out)
        << "        <block SourceFile=\"" << source2.GetFilename()
        << "\" StartLineNumber=\"" << startLineNumber2
        << "\" EndLineNumber=\"" << endLineNumber2 << "\"/>"
        << std::endl;
    (*m_out)
        << "        <lines xml:space=\"preserve\">"
        << std::endl;
    for (int j = 0; j < count; j++) {
        // replace various characters/ strings so that it doesn't upset the XML parser
        std::string tmpstr = source1.GetLine(j + line1).GetLine();

        // " --> '
        StringUtil::StrSub(tmpstr, "\'", "\"", -1);

        // & --> &amp;
        StringUtil::StrSub(tmpstr, "&amp;", "&", -1);

        // < --> &lt;
        StringUtil::StrSub(tmpstr, "&lt;", "<", -1);

        // > --> &gt;
        StringUtil::StrSub(tmpstr, "&gt;", ">", -1);

        (*m_out) << "            <line Text=\"" << tmpstr << "\"/>" << std::endl;
    }

    (*m_out) << "        </lines>" << std::endl;
    (*m_out) << "    </set>" << std::endl;
}
