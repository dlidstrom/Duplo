#ifndef _IEXPORTER_H_
#define _IEXPORTER_H_

#include "Block.h"
#include "Options.h"
#include "SourceFile.h"

#include <memory>

struct IExporter;
typedef std::shared_ptr<IExporter> IExporterPtr;

struct IExporter {
    virtual ~IExporter() = default;
    static IExporterPtr CreateExporter(const Options& options);
    virtual void LogMessage(const std::string& message) = 0;
    virtual void WriteHeader() = 0;
    virtual void WriteFooter(
        const Options& options,
        int files,
        long locsTotal,
        std::vector<Block> const &tot_dup_blocks,
        unsigned tot_dup_lines) = 0;
    virtual void ReportSeq(
        int line1,
        int line2,
        int count,
        const SourceFile& source1,
        const SourceFile& source2) = 0;
};

#endif
