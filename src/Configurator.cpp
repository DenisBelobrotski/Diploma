#include "Configurator.h"

#include "ConfigJsonFileReader.h"


diploma::Configurator::Configurator() : configFileReader(nullptr) {}


diploma::Configurator::Configurator(const std::string& filePath, ConfigFileType fileType)
{
    switch (fileType)
    {
        case ConfigFileTypeJson:
            configFileReader = new ConfigJsonFileReader(filePath);
            break;

        default:
            configFileReader = nullptr;
    }
}


diploma::Configurator::~Configurator()
{
    delete configFileReader;
}


bool diploma::Configurator::isNeedPauseExecutionBeforeTermination()
{
    return configFileReader->isNeedPauseExecutionBeforeTermination();
}


bool diploma::Configurator::isNonUniformConcentration()
{
    return configFileReader->isNonUniformConcentration();
}


bool diploma::Configurator::isNeedCalculateResiduals()
{
    return configFileReader->isNeedCalculateResiduals();
}


bool diploma::Configurator::isNeedMakeComparisonPlot()
{
    return configFileReader->isNeedMakeComparisonPlot();
}


bool diploma::Configurator::isComparisonUniformNonUniform()
{
    return configFileReader->isComparisonUniformNonUniform();
}
