#include "Diploma.h"
#include "Utils.h"
#include "DiplomaPlotHelper.h"

#include <iostream>
#include <chrono>
#include <future>


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
