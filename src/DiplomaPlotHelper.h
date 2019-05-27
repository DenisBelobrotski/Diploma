#pragma once

#include <GnuplotWrapper/Plot.h>
#include <MagneticFluidFormAlgorithm/DifferenceMethod.h>


namespace diploma
{
    plot::Plot* configMagneticFluidPlot(
            std::vector<algorithm::IterationInfo>& iterationsInfo, std::string title,
            plot::PlotOutputType outputType, std::string outputFilePath);

    plot::Plot* configComparisonPlot(
            algorithm::IterationInfo& firstExperimentIterationInfo,
            algorithm::IterationInfo& secondExperimentIterationInfo,
            std::string title, plot::PlotOutputType outputType, std::string outputFilePath);

    void fillGraphTitleStreamDefault(
            std::stringstream& titleStream, const std::string& title, algorithm::IterationInfo& iterationInfo);

    void configDefaultPlot(plot::PlotConfig& config);

    plot::Plot* createDefaultPlot(
            std::vector<plot::Graph>* graphs, std::string title, plot::PlotOutputType outputType,
            std::string outputFilePath);
}
