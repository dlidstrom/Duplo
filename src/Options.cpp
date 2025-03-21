#include "Options.h"

Options::Options(
    unsigned minChars,
    bool ignorePrepStuff,
    unsigned minBlockSize,
    unsigned blockPercentThreshold,
    unsigned filesToCheck,
    bool outputXml,
    bool outputJSON,
    bool ignoreSameFilename,
    const std::string& listFilename,
    const std::string& outputFilename)
    : m_minChars(minChars)
    , m_ignorePrepStuff(ignorePrepStuff)
    , m_minBlockSize(minBlockSize)
    , m_blockPercentThreshold(blockPercentThreshold)
    , m_filesToCheck(filesToCheck)
    , m_outputXml(outputXml)
    , m_outputJSON(outputJSON)
    , m_ignoreSameFilename(ignoreSameFilename)
    , m_listFilename(listFilename)
    , m_outputFilename(outputFilename)
{
}

unsigned Options::GetMinChars() const {
    return m_minChars;
}

bool Options::GetIgnorePrepStuff() const {
    return m_ignorePrepStuff;
}

unsigned Options::GetMinBlockSize() const {
    return m_minBlockSize;
}

unsigned Options::GetBlockPercentThreshold() const {
    return m_blockPercentThreshold;
}

bool Options::GetOutputXml() const {
    return m_outputXml;
}

bool Options::GetOutputJSON() const {
    return m_outputJSON;
}

bool Options::GetIgnoreSameFilename() const {
    return m_ignoreSameFilename;
}

const std::string& Options::GetListFilename() const {
    return m_listFilename;
}

const std::string& Options::GetOutputFilename() const {
    return m_outputFilename;
}

size_t Options::GetFilesToCheck() const {
    return m_filesToCheck;
}
