#include "Diploma.h"
#include "Utils.h"

#include <sstream>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <future>

#include <GnuplotWrapper/GnuplotWrapper.h>
#include <MagneticFluidFormAlgorithm/MagneticFluidFormAlgorithm.h>


plot::Plot* configMagneticFluidPlot(
        std::vector<algorithm::Variables>& iterationVariables, std::vector<algorithm::IterationInfo>& iterationsInfo,
        std::string title, plot::PlotOutputType outputType, std::string outputFilePath);

plot::Plot* configAnglesPlot(
        std::vector<algorithm::Variables>& iterationVariables, std::vector<algorithm::IterationInfo>& iterationsInfo);

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

void calcResults(
        std::vector<algorithm::Variables>& implicitExperimentVariables,
        std::vector<algorithm::Variables>& explicitExperimentVariables,
        std::vector<algorithm::IterationInfo>& implicitIterationsInfo,
        std::vector<algorithm::IterationInfo>& explicitIterationsInfo);

void makePlots(
        std::vector<algorithm::Variables>& implicitExperimentVariables,
        std::vector<algorithm::Variables>& explicitExperimentVariables,
        std::vector<algorithm::IterationInfo>& implicitIterationsInfo,
        std::vector<algorithm::IterationInfo>& explicitIterationsInfo,
        bool needPauseExecution);

void calcResiduals(
        std::vector<algorithm::Variables>& implicitExperimentVariables,
        std::vector<algorithm::Variables>& explicitExperimentVariables,
        std::vector<algorithm::IterationInfo>& implicitIterationsInfo,
        std::vector<algorithm::IterationInfo>& explicitIterationsInfo);

void printTotalDuration(
        std::chrono::time_point<std::chrono::system_clock>& start,
        std::chrono::time_point<std::chrono::system_clock>& end);


int main()
{
    try
    {
        std::vector<algorithm::Variables> implicitExperimentVariables;
        std::vector<algorithm::Variables> explicitExperimentVariables;

        std::vector<algorithm::IterationInfo> implicitIterationsInfo;
        std::vector<algorithm::IterationInfo> explicitIterationsInfo;

        calcResults(
                implicitExperimentVariables, explicitExperimentVariables, implicitIterationsInfo,
                explicitIterationsInfo);

        calcResiduals(
                implicitExperimentVariables, explicitExperimentVariables, implicitIterationsInfo,
                explicitIterationsInfo);

        makePlots(implicitExperimentVariables, explicitExperimentVariables, implicitIterationsInfo,
                  explicitIterationsInfo, true);
    }
    catch (std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}


plot::Plot* configMagneticFluidPlot(
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


plot::Plot* configAnglesPlot(
        std::vector<algorithm::Variables>& iterationVariables, std::vector<algorithm::IterationInfo>& iterationsInfo)
{
    return nullptr;
}


void configPlotAxesRanges(plot::AxesRanges& axesRanges, std::vector<plot::Graph>& graphs)
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


plot::Plot* configComparisonPlot(
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


void fillGraphTitleStreamDefault(
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


void configDefaultPlot(plot::PlotConfig& config)
{
    config.windowWidth = 1600;
    config.windowHeight = 900;
    config.xAxisName = "Radius";
    config.yAxisName = "Height";
    config.legendFontSize = 8;
    config.equalAxes = true;
}


plot::Plot* createDefaultPlot(
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


void calcResults(
        std::vector<algorithm::Variables>& implicitExperimentVariables,
        std::vector<algorithm::Variables>& explicitExperimentVariables,
        std::vector<algorithm::IterationInfo>& implicitIterationsInfo,
        std::vector<algorithm::IterationInfo>& explicitIterationsInfo)
{
    algorithm::DifferenceMethod* implicitDifferenceMethod = new algorithm::ImplicitDifferenceMethod(
            &implicitExperimentVariables, &implicitIterationsInfo);
    algorithm::DifferenceMethod* explicitDifferenceMethod = new algorithm::ExplicitDifferenceMethod(
            &explicitExperimentVariables, &explicitIterationsInfo);

    implicitDifferenceMethod->setIsNeedResetTau(false);
    explicitDifferenceMethod->setIsNeedResetTau(false);

    std::function<void(long long, long long)> showIterationsProgressBarFunction =
            [](long long currentIteration, long long maxIterations)
            {
                utils::showIterationsProgressBar(currentIteration, maxIterations);
            };
    implicitDifferenceMethod->setIterationFinishedCallback(&showIterationsProgressBarFunction);
    explicitDifferenceMethod->setIterationFinishedCallback(&showIterationsProgressBarFunction);

    algorithm::AlgorithmConfigurator algorithmConfigurator;
    std::vector<algorithm::TargetParameter>* targetParameters =
            algorithmConfigurator.readAlgorithmSequenceFromFile(
                    "../res/algorithm_config_full.json", algorithm::ConfigFileTypeJson);

    implicitDifferenceMethod->setTargetParameters(targetParameters);
    explicitDifferenceMethod->setTargetParameters(targetParameters);

    std::cout << "*****Algorithms info*****" << std::endl;

    std::chrono::time_point<std::chrono::system_clock> start;
    std::chrono::time_point<std::chrono::system_clock> end;

    start = std::chrono::system_clock::now();

    std::future<void> implicitDifferenceMethodCalculation(std::async(
            [&implicitDifferenceMethod]()
            {
                std::chrono::time_point<std::chrono::system_clock> start;
                std::chrono::time_point<std::chrono::system_clock> end;

                start = std::chrono::system_clock::now();

                std::cout << "Finite-difference method (start): " << std::endl << std::endl;

                implicitDifferenceMethod->calcResult();

                end = std::chrono::system_clock::now();

                std::cout << "Finite-difference method (finish): " << std::endl;
                printTotalDuration(start, end);
            }));

    std::future<void> explicitDifferenceMethodCalculation(std::async(
            [&explicitDifferenceMethod]()
            {
                std::chrono::time_point<std::chrono::system_clock> start;
                std::chrono::time_point<std::chrono::system_clock> end;

                start = std::chrono::system_clock::now();

                std::cout << "Tangential method (start): " << std::endl;
                explicitDifferenceMethod->calcResult();

                end = std::chrono::system_clock::now();

                std::cout << "Tangential method (finish): " << std::endl;
                printTotalDuration(start, end);
            }));

    implicitDifferenceMethodCalculation.get();
    explicitDifferenceMethodCalculation.get();

    end = std::chrono::system_clock::now();

    std::cout << "All calculations results: " << std::endl;
    printTotalDuration(start, end);

    delete implicitDifferenceMethod;
    delete explicitDifferenceMethod;
}


void makePlots(
        std::vector<algorithm::Variables>& implicitExperimentVariables,
        std::vector<algorithm::Variables>& explicitExperimentVariables,
        std::vector<algorithm::IterationInfo>& implicitIterationsInfo,
        std::vector<algorithm::IterationInfo>& explicitIterationsInfo,
        bool needPauseExecution)
{
    plot::Plot* implicitMethodPlot = configMagneticFluidPlot(
            implicitExperimentVariables, implicitIterationsInfo, "Finite-difference method",
            plot::PlotOutputTypeWindow, "");
    plot::Plot* explicitMethodPlot = configMagneticFluidPlot(
            explicitExperimentVariables, explicitIterationsInfo, "Tangential method", plot::PlotOutputTypeWindow,
            "");
    plot::Plot* comparisonPlot = configComparisonPlot(
            implicitExperimentVariables.back(), implicitIterationsInfo.back(), explicitExperimentVariables.back(),
            explicitIterationsInfo.back(), "Comparison", plot::PlotOutputTypeWindow, "");

    implicitMethodPlot->makeGraphs();
    explicitMethodPlot->makeGraphs();
    comparisonPlot->makeGraphs();

    if (needPauseExecution)
    {
        utils::pauseExecution();
    }

    delete explicitMethodPlot;
    delete implicitMethodPlot;
    delete comparisonPlot;
}


void calcResiduals(
        std::vector<algorithm::Variables>& implicitExperimentVariables,
        std::vector<algorithm::Variables>& explicitExperimentVariables,
        std::vector<algorithm::IterationInfo>& implicitIterationsInfo,
        std::vector<algorithm::IterationInfo>& explicitIterationsInfo)
{
    std::cout << "*****Residuals*****" << std::endl;
    auto size = std::min(explicitIterationsInfo.size(), implicitIterationsInfo.size());
    for (auto i = 0; i < size; i++)
    {
        auto currentOldIteration = implicitIterationsInfo[i].index;
        auto currentNewIteration = explicitIterationsInfo[i].index;

        double radiusResidual = algorithm::calcResidual(implicitExperimentVariables[currentOldIteration].r,
                                                        explicitExperimentVariables[currentNewIteration].r);
        double heightResidual = algorithm::calcResidual(implicitExperimentVariables[currentOldIteration].z,
                                                        explicitExperimentVariables[currentNewIteration].z);
        double commonResidual = std::max(radiusResidual, heightResidual);

        std::cout << "Experiment number:" << std::endl;
        std::cout << "Finite-difference method: #" << currentOldIteration << std::endl;
        std::cout << "Tangential method: #" << currentNewIteration << std::endl;
        std::cout << "Residual:" << std::endl;
        std::cout << "radius: " << radiusResidual << ", height: " << heightResidual << std::endl;
        std::cout << "common: " << commonResidual << std::endl;
        std::cout << "************************" << std::endl;
    }
}


void printTotalDuration(
        std::chrono::time_point<std::chrono::system_clock>& start,
        std::chrono::time_point<std::chrono::system_clock>& end)
{
    auto totalCalculationsTimeMillis = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    auto totalCalculationsTimeSecs = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

    std::cout << "Total calculations time: " << totalCalculationsTimeMillis << " ms." << " ("
              << totalCalculationsTimeSecs << " sec.)" << std::endl;
}
