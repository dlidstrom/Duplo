#include "FileExporter.h"

#include <iostream>
#include <sstream>

FileExporter::FileExporter(const Options& options, bool verbose) {
    std::streambuf* buf;
    std::streambuf* logbuf;
    if (options.GetOutputFilename() == "-") {
        buf = std::cout.rdbuf();
        if (verbose) {
            logbuf = std::cout.rdbuf();
        } else {
            logbuf = 0;
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

std::ostream& FileExporter::Log() const {
    return *m_log;
}

std::ostream& FileExporter::Out() const {
    return *m_out;
}
