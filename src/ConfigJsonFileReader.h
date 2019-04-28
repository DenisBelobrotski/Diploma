#pragma once

#include "ConfigFileReader.h"

#include <rapidjson/document.h>

namespace diploma
{
    class ConfigJsonFileReader : public ConfigFileReader
    {
    private:
        rapidjson::Document* document;
    public:
        ConfigJsonFileReader();

        explicit ConfigJsonFileReader(const std::string& filePath);

        ~ConfigJsonFileReader() override;

        bool isNeedPauseExecutionBeforeTermination() override;

        bool isNonUniformConcentration() override;

        bool isNeedCalculateResiduals() override;

        bool isNeedMakeComparisonPlot() override;
    };
}
