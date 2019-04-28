#pragma once

#include <GnuplotWrapper/GnuplotWrapper.h>
#include <MagneticFluidFormAlgorithm/DifferenceMethod.h>


namespace diploma
{
    plot::Plot* configMagneticFluidPlot(
            std::vector<algorithm::Variables>& iterationVariables,
            std::vector<algorithm::IterationInfo>& iterationsInfo,
            std::string title, plot::PlotOutputType outputType, std::string outputFilePath);

    void configPlotAxesRanges(plot::AxesRanges& axesRanges, std::vector<plot::Graph>& graphs);

    plot::Plot* configComparisonPlot(
            algorithm::Variables& oldIterationVariables, algorithm::IterationInfo& firstExperimentIterationInfo,
            algorithm::Variables& secondExperimentVariables, algorithm::IterationInfo& secondExperimentIterationInfo,
            std::string title, plot::PlotOutputType outputType, std::string outputFilePath);

    void fillGraphTitleStreamDefault(
            std::stringstream& titleStream, const std::string& title, algorithm::IterationInfo& iterationInfo);

    void configDefaultPlot(plot::PlotConfig& config);

    plot::Plot* createDefaultPlot(
            std::vector<plot::Graph>* graphs, std::string title, plot::PlotOutputType outputType,
            std::string outputFilePath);
}
