#include "IExporter.h"
#include "ConsoleExporter.h"
#include "JsonExporter.h"
#include "XmlExporter.h"

IExporterPtr IExporter::CreateExporter(const Options& options) {
    IExporterPtr exporter;
    if (options.GetOutputXml()) {
        if (options.GetOutputJSON())
            throw std::invalid_argument("Specify a single output format");
        exporter = std::make_shared<XmlExporter>(options);
    } else if (options.GetOutputJSON()) {
        exporter = std::make_shared<JsonExporter>(options);
    } else {
        exporter = std::make_shared<ConsoleExporter>(options);
    }

    return exporter;
}
