#include "DiplomaPlotHelper.h"

#include <sstream>

#include <GnuplotWrapper/Utils.h>


plot::Plot* diploma::configMagneticFluidPlot(
        std::vector<algorithm::IterationInfo>& iterationsInfo, std::string title,
        plot::PlotOutputType outputType, std::string outputFilePath)
{
    auto graphs = new std::vector<plot::Graph>();

    for (auto& currentIterationInfo : iterationsInfo)
    {
        std::vector<plot::Point> points;
        plot::convertComponentsVectorsToPointsVector(
                currentIterationInfo.variables.r, currentIterationInfo.variables.z, points);

        std::stringstream titleStream;
        titleStream << "#" << currentIterationInfo.index;
        fillGraphTitleStreamDefault(titleStream, "", currentIterationInfo);

        plot::Graph graph;
        graph.title = titleStream.str();
        graph.points = points;

        graphs->push_back(graph);
    }

    return createDefaultPlot(graphs, std::move(title), outputType, std::move(outputFilePath));
}


plot::Plot* diploma::configComparisonPlot(
        algorithm::IterationInfo& firstExperimentIterationInfo,
        algorithm::IterationInfo& secondExperimentIterationInfo,
        std::string title, plot::PlotOutputType outputType, std::string outputFilePath)
{
    auto graphs = new std::vector<plot::Graph>();

    std::vector<plot::Point> firstExperimentPoints;
    plot::convertComponentsVectorsToPointsVector(
            firstExperimentIterationInfo.variables.r, firstExperimentIterationInfo.variables.z,
            firstExperimentPoints);

    std::stringstream firstExperimentTitleStream;
    fillGraphTitleStreamDefault(firstExperimentTitleStream, "First experiment", firstExperimentIterationInfo);

    plot::Graph firstExperimentGraph;
    firstExperimentGraph.title = firstExperimentTitleStream.str();
    firstExperimentGraph.points = firstExperimentPoints;

    graphs->push_back(firstExperimentGraph);

    std::vector<plot::Point> secondExperimentPoints;
    plot::convertComponentsVectorsToPointsVector(
            secondExperimentIterationInfo.variables.r, secondExperimentIterationInfo.variables.z,
            secondExperimentPoints);

    std::stringstream secondExperimentTitleStream;
    fillGraphTitleStreamDefault(secondExperimentTitleStream, "Second experiment", secondExperimentIterationInfo);

    plot::Graph secondExperimentGraph;
    secondExperimentGraph.title = secondExperimentTitleStream.str();
    secondExperimentGraph.points = secondExperimentPoints;

    graphs->push_back(secondExperimentGraph);

    return createDefaultPlot(graphs, std::move(title), outputType, std::move(outputFilePath));
}


void diploma::fillGraphTitleStreamDefault(
        std::stringstream& titleStream, const std::string& title, algorithm::IterationInfo& iterationInfo)
{
    const bool isNeedShowFullInfoInLegend = false;

    titleStream << title << " - ";

    if (isNeedShowFullInfoInLegend)
    {
        titleStream << "TAU: " << iterationInfo.variables.TAU
                    << ", U: " << iterationInfo.variables.U
                    << ", B0: " << iterationInfo.variables.B0
                    << ", A1: " << iterationInfo.variables.A1
                    << ", A2: " << iterationInfo.variables.A2
                    << ", ALPHA: " << iterationInfo.variables.ALPHA;
    }
    else
    {
        titleStream << iterationInfo.mainValueName << ": " << iterationInfo.getMainDoubleValue();
    }
}


void diploma::configDefaultPlot(plot::PlotConfig& config)
{
    config.windowWidth = 1600;
    config.windowHeight = 900;
    config.xAxisName = "Radius";
    config.yAxisName = "Height";
    config.legendFontSize = 8;
    config.equalAxes = true;
}


plot::Plot* diploma::createDefaultPlot(
        std::vector<plot::Graph>* graphs, std::string title, plot::PlotOutputType outputType,
        std::string outputFilePath)
{
    plot::AxesRanges axesRanges{plot::Range(0, 0), plot::Range(0, 0)};
    auto config = new plot::PlotConfig();
    plot::optimizeRangesBounds(axesRanges, *graphs);
    configDefaultPlot(*config);
    config->title = std::move(title);
    config->axesRanges = axesRanges;
    config->outputType = outputType;
    config->outputFilePath = std::move(outputFilePath);

    return new plot::Plot(config, graphs);
}
