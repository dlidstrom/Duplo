#ifndef _FILEEXPORTER_H_
#define _FILEEXPORTER_H_

#include "IExporter.h"

#include <fstream>

class FileExporter : public IExporter {
    std::ofstream m_of;
    std::shared_ptr<std::ostream> m_out;
    std::shared_ptr<std::ostream> m_log;

protected:
    std::ostream& Log() const;
    std::ostream& Out() const;

public:
    FileExporter(const Options&, bool);
};

#endif
