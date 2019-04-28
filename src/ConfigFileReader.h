#pragma once


namespace diploma
{
    const auto DIPLOMA_CONFIG_KEY = "diplomaConfig";

    const auto IS_NEED_PAUSE_EXECUTION_BEFORE_TERMINATION_KEY = "isNeedPauseExecutionBeforeTermination";
    const auto IS_NON_UNIFORM_CONCENTRATION_KEY = "isNonUniformConcentration";
    const auto IS_NEED_CALCULATE_RESIDUALS_KEY = "isNeedCalculateResiduals";
    const auto IS_NEED_MAKE_COMPARISON_PLOT_KEY = "isNeedMakeComparisonPlot";
    const auto IS_COMPARISON_UNIFORM_NON_UNIFORM = "isComparisonUniformNonUniform";


    class ConfigFileReader
    {
    public:
        ConfigFileReader() = default;

        virtual ~ConfigFileReader() = default;

        virtual bool isNeedPauseExecutionBeforeTermination() = 0;

        virtual bool isNonUniformConcentration() = 0;

        virtual bool isNeedCalculateResiduals() = 0;

        virtual bool isNeedMakeComparisonPlot() = 0;

        virtual bool isComparisonUniformNonUniform() = 0;
    };
}
