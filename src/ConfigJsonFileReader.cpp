#include "ConfigJsonFileReader.h"

#include <fstream>
#include <sstream>

#include "Exceptions.h"


diploma::ConfigJsonFileReader::ConfigJsonFileReader() : ConfigFileReader(), document(nullptr) {}


diploma::ConfigJsonFileReader::ConfigJsonFileReader(const std::string& filePath)
{
    std::ifstream configFile(filePath);
    std::stringstream jsonBuffer;
    jsonBuffer << configFile.rdbuf();

    document = new rapidjson::Document();
    document->Parse(jsonBuffer.str().c_str());

    if (!document->IsObject())
    {
        delete document;
        throw IncorrectConfigException("Json document does not contain any object");
    }

    if (!document->HasMember(DIPLOMA_CONFIG_KEY))
    {
        delete document;
        throw IncorrectConfigException("Json document does not contain diploma config");
    }
}


diploma::ConfigJsonFileReader::~ConfigJsonFileReader()
{
    delete document;
}


bool diploma::ConfigJsonFileReader::isNeedPauseExecutionBeforeTermination()
{
    auto diplomaConfigObject = (*document)[DIPLOMA_CONFIG_KEY].GetObject();

    bool result = false;

    if (diplomaConfigObject.HasMember(IS_NEED_PAUSE_EXECUTION_BEFORE_TERMINATION_KEY))
    {
        result = diplomaConfigObject[IS_NEED_PAUSE_EXECUTION_BEFORE_TERMINATION_KEY].GetBool();
    }

    return result;
}


bool diploma::ConfigJsonFileReader::isNonUniformConcentration()
{
    auto diplomaConfigObject = (*document)[DIPLOMA_CONFIG_KEY].GetObject();

    bool result = false;

    if (diplomaConfigObject.HasMember(IS_NON_UNIFORM_CONCENTRATION_KEY))
    {
        result = diplomaConfigObject[IS_NON_UNIFORM_CONCENTRATION_KEY].GetBool();
    }

    return result;
}


bool diploma::ConfigJsonFileReader::isNeedCalculateResiduals()
{
    auto diplomaConfigObject = (*document)[DIPLOMA_CONFIG_KEY].GetObject();

    bool result = false;

    if (diplomaConfigObject.HasMember(IS_NEED_CALCULATE_RESIDUALS_KEY))
    {
        result = diplomaConfigObject[IS_NEED_CALCULATE_RESIDUALS_KEY].GetBool();
    }

    return result;
}


bool diploma::ConfigJsonFileReader::isNeedMakeComparisonPlot()
{
    auto diplomaConfigObject = (*document)[DIPLOMA_CONFIG_KEY].GetObject();

    bool result = false;

    if (diplomaConfigObject.HasMember(IS_NEED_MAKE_COMPARISON_PLOT_KEY))
    {
        result = diplomaConfigObject[IS_NEED_MAKE_COMPARISON_PLOT_KEY].GetBool();
    }

    return result;
}


bool diploma::ConfigJsonFileReader::isComparisonUniformNonUniform()
{
    auto diplomaConfigObject = (*document)[DIPLOMA_CONFIG_KEY].GetObject();

    bool result = false;

    if (diplomaConfigObject.HasMember(IS_COMPARISON_UNIFORM_NON_UNIFORM))
    {
        result = diplomaConfigObject[IS_COMPARISON_UNIFORM_NON_UNIFORM].GetBool();
    }

    return result;
}
