#include "DiplomaPlotHelper.h"

#include <sstream>


plot::Plot* diploma::configMagneticFluidPlot(
        std::vector<algorithm::Variables>& iterationVariables, std::vector<algorithm::IterationInfo>& iterationsInfo,
        std::string title, plot::PlotOutputType outputType, std::string outputFilePath)
{
    auto graphs = new std::vector<plot::Graph>();

    for (auto i = 0; i < iterationsInfo.size(); i++)
    {
        long long currentIteration = iterationsInfo[i].index;

        std::vector<plot::Point> points;
        plot::convertComponentsVectorsToPointsVector(iterationVariables[currentIteration].r,
                                                     iterationVariables[currentIteration].z,
                                                     points);

        std::stringstream titleStream;
        titleStream << "#" << currentIteration;
        fillGraphTitleStreamDefault(titleStream, "", iterationsInfo[i]);

        plot::Graph graph;
        graph.title = titleStream.str();
        graph.points = points;

        graphs->push_back(graph);
    }

    return createDefaultPlot(graphs, title, outputType, outputFilePath);
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
        algorithm::Variables& oldIterationVariables, algorithm::IterationInfo& oldIterationInfo,
        algorithm::Variables& newIterationVariables, algorithm::IterationInfo& newIterationInfo,
        std::string title, plot::PlotOutputType outputType, std::string outputFilePath)
{
    auto graphs = new std::vector<plot::Graph>();

    std::vector<plot::Point> oldPoints;
    plot::convertComponentsVectorsToPointsVector(oldIterationVariables.r,
                                                 oldIterationVariables.z,
                                                 oldPoints);

    std::stringstream oldTitleStream;
    fillGraphTitleStreamDefault(oldTitleStream, "Old algorithm", oldIterationInfo);

    plot::Graph oldGraph;
    oldGraph.title = oldTitleStream.str();
    oldGraph.points = oldPoints;

    graphs->push_back(oldGraph);

    std::vector<plot::Point> newPoints;
    plot::convertComponentsVectorsToPointsVector(newIterationVariables.r,
                                                 newIterationVariables.z,
                                                 newPoints);

    std::stringstream newTitleStream;
    fillGraphTitleStreamDefault(newTitleStream, "New algorithm", newIterationInfo);

    plot::Graph newGraph;
    newGraph.title = newTitleStream.str();
    newGraph.points = newPoints;

    graphs->push_back(newGraph);

    return createDefaultPlot(graphs, title, outputType, outputFilePath);
}


void diploma::fillGraphTitleStreamDefault(
        std::stringstream& titleStream, std::string title, algorithm::IterationInfo& iterationInfo)
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
    config->title = title;
    config->axesRanges = axesRanges;
    config->outputType = outputType;
    config->outputFilePath = outputFilePath;

    return new plot::Plot(config, graphs);
}
