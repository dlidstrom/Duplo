#ifndef _XMLEXPORTER_H_
#define _XMLEXPORTER_H_

#include "FileExporter.h"

class XmlExporter : public FileExporter {
public:
    XmlExporter(const Options& options);
    void LogMessage(const std::string& message) override;
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
