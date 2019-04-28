#pragma once


namespace diploma
{
    const auto DIPLOMA_CONFIG_KEY = "diplomaConfig";

    const auto IS_NEED_PAUSE_EXECUTION_BEFORE_TERMINATION_KEY = "isNeedPauseExecutionBeforeTermination";
    const auto IS_NON_UNIFORM_CONCENTRATION_KEY = "isNonUniformConcentration";
    const auto IS_NEED_CALCULATE_RESIDUALS_KEY = "isNeedCalculateResiduals";
    const auto IS_NEED_MAKE_COMPARISON_PLOT_KEY = "isNeedMakeComparisonPlot";


    class ConfigFileReader
    {
    public:
        ConfigFileReader() = default;

        virtual ~ConfigFileReader() = default;

        virtual bool isNeedPauseExecutionBeforeTermination() = 0;

        virtual bool isNonUniformConcentration() = 0;

        virtual bool isNeedCalculateResiduals() = 0;

        virtual bool isNeedMakeComparisonPlot() = 0;
    };
}
