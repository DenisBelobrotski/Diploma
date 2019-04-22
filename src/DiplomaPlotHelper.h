#pragma once

#include <GnuplotWrapper/GnuplotWrapper.h>
#include <MagneticFluidFormAlgorithm/MagneticFluidFormAlgorithm.h>


plot::Plot* configMagneticFluidPlot(
        std::vector<algorithm::Variables>& iterationVariables, std::vector<algorithm::IterationInfo>& iterationsInfo,
        std::string title, plot::PlotOutputType outputType, std::string outputFilePath);

void configPlotAxesRanges(plot::AxesRanges& axesRanges, std::vector<plot::Graph>& graphs);

plot::Plot* configComparisonPlot(
        algorithm::Variables& oldIterationVariables, algorithm::IterationInfo& oldIterationInfo,
        algorithm::Variables& newIterationVariables, algorithm::IterationInfo& newIterationInfo,
        std::string title, plot::PlotOutputType outputType, std::string outputFilePath);

void fillGraphTitleStreamDefault(
        std::stringstream& titleStream, std::string title, algorithm::IterationInfo& iterationInfo);

void configDefaultPlot(plot::PlotConfig& config);

plot::Plot* createDefaultPlot(
        std::vector<plot::Graph>* graphs, std::string title, plot::PlotOutputType outputType,
        std::string outputFilePath);
