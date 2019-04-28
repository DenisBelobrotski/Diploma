#include "DiplomaPlotHelper.h"

#include <sstream>


plot::Plot* diploma::configMagneticFluidPlot(
        std::vector<algorithm::Variables>& iterationVariables, std::vector<algorithm::IterationInfo>& iterationsInfo,
        std::string title, plot::PlotOutputType outputType, std::string outputFilePath)
{
    auto graphs = new std::vector<plot::Graph>();

    for (auto& currentIterationInfo : iterationsInfo)
    {
        long long currentIteration = currentIterationInfo.index;

        std::vector<plot::Point> points;
        plot::convertComponentsVectorsToPointsVector(iterationVariables[currentIteration].r,
                                                     iterationVariables[currentIteration].z,
                                                     points);

        std::stringstream titleStream;
        titleStream << "#" << currentIteration;
        fillGraphTitleStreamDefault(titleStream, "", currentIterationInfo);

        plot::Graph graph;
        graph.title = titleStream.str();
        graph.points = points;

        graphs->push_back(graph);
    }

    return createDefaultPlot(graphs, std::move(title), outputType, std::move(outputFilePath));
}


void diploma::configPlotAxesRanges(plot::AxesRanges& axesRanges, std::vector<plot::Graph>& graphs)
{

    for (auto& currentGraph : graphs)
    {
        for (auto& currentPoint : currentGraph.points)
        {
            axesRanges.xAxisRange.end = std::max(axesRanges.xAxisRange.end, currentPoint.x);
            axesRanges.yAxisRange.end = std::max(axesRanges.yAxisRange.end, currentPoint.y);
        }
    }
    axesRanges.xAxisRange.end = std::ceil(axesRanges.xAxisRange.end);
    axesRanges.yAxisRange.end = std::ceil(axesRanges.yAxisRange.end);
}


plot::Plot* diploma::configComparisonPlot(
        algorithm::Variables& firstExperimentVariables, algorithm::IterationInfo& firstExperimentIterationInfo,
        algorithm::Variables& secondExperimentVariables, algorithm::IterationInfo& secondExperimentIterationInfo,
        std::string title, plot::PlotOutputType outputType, std::string outputFilePath)
{
    auto graphs = new std::vector<plot::Graph>();

    std::vector<plot::Point> oldPoints;
    plot::convertComponentsVectorsToPointsVector(firstExperimentVariables.r,
                                                 firstExperimentVariables.z,
                                                 oldPoints);

    std::stringstream oldTitleStream;
    fillGraphTitleStreamDefault(oldTitleStream, "First experiment", firstExperimentIterationInfo);

    plot::Graph oldGraph;
    oldGraph.title = oldTitleStream.str();
    oldGraph.points = oldPoints;

    graphs->push_back(oldGraph);

    std::vector<plot::Point> newPoints;
    plot::convertComponentsVectorsToPointsVector(secondExperimentVariables.r,
                                                 secondExperimentVariables.z,
                                                 newPoints);

    std::stringstream newTitleStream;
    fillGraphTitleStreamDefault(newTitleStream, "Second experiment", secondExperimentIterationInfo);

    plot::Graph newGraph;
    newGraph.title = newTitleStream.str();
    newGraph.points = newPoints;

    graphs->push_back(newGraph);

    return createDefaultPlot(graphs, std::move(title), outputType, std::move(outputFilePath));
}


void diploma::fillGraphTitleStreamDefault(
        std::stringstream& titleStream, const std::string& title, algorithm::IterationInfo& iterationInfo)
{
    titleStream << title << " - "
                << "TAU: " << iterationInfo.tau
                << ", U: " << iterationInfo.u
                << ", B0: " << iterationInfo.b0
                << ", A1: " << iterationInfo.a1
                << ", A2: " << iterationInfo.a2
                << ", ALPHA: " << iterationInfo.alpha;
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
    plot::AxesRanges axesRanges{plot::Range(1, 0), plot::Range(0, 0)};
    auto config = new plot::PlotConfig();
    configPlotAxesRanges(axesRanges, *graphs);
    configDefaultPlot(*config);
    config->title = std::move(title);
    config->axesRanges = axesRanges;
    config->outputType = outputType;
    config->outputFilePath = std::move(outputFilePath);

    return new plot::Plot(config, graphs);
}
