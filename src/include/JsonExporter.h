#ifndef _JSONEXPORTER_H_
#define _JSONEXPORTER_H_

#include "IExporter.h"

#include <nlohmann/json.hpp>

#include <fstream>

class JsonExporter : public IExporter {
    std::ofstream m_of;
    std::shared_ptr<std::ostream> m_out;
    std::shared_ptr<std::ostream> m_log;
    nlohmann::json m_json;

public:
    JsonExporter(const Options& options);
    void Log(const std::string& message) override;
    void WriteHeader() override;
    void WriteFooter(
        const Options& options,
        int files,
        long locsTotal,
        const ProcessResult& processResult) override;
    void ReportSeq(
        int line1,
        int line2,
        int count,
        const SourceFile& source1,
        const SourceFile& source2) override;
};

#endif
