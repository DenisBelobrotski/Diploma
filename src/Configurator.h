#pragma once

#include "ConfigFileReader.h"

#include <string>


namespace diploma
{
    enum ConfigFileType
    {
        ConfigFileTypeNone,
        ConfigFileTypeJson
    };

    class Configurator
    {
    private:
        ConfigFileReader* configFileReader;
    public:
        Configurator();

        Configurator(const std::string& filePath, ConfigFileType fileType);

        ~Configurator();

        bool isNeedPauseExecutionBeforeTermination();

        bool isNonUniformConcentration();

        bool isNeedCalculateResiduals();

        bool isNeedMakeComparisonPlot();

        bool isComparisonUniformNonUniform();
    };
}
