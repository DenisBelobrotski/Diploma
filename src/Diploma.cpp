#include "Diploma.h"
#include "Utils.h"
#include "DiplomaPlotHelper.h"

#include <iostream>
#include <chrono>
#include <future>

#include <MagneticFluidFormAlgorithm/AlgorithmConfigurator.h>
#include <MagneticFluidFormAlgorithm/DifferenceMethod.h>
#include <MagneticFluidFormAlgorithm/ExplicitDifferenceMethod.h>
#include <MagneticFluidFormAlgorithm/ImplicitDifferenceMethod.h>
#include <MagneticFluidFormAlgorithm/ExplicitMethodUniformConcentration.h>
#include <MagneticFluidFormAlgorithm/ImplicitMethodUniformConcentration.h>
#include <MagneticFluidFormAlgorithm/MathUtils.h>


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
        std::chrono::time_point<std::chrono::steady_clock>& start,
        std::chrono::time_point<std::chrono::steady_clock>& end);


int main()
{
    try
    {
        const bool isNeedCalculateResiduals = false;
        std::vector<algorithm::Variables> implicitExperimentVariables;
        std::vector<algorithm::Variables> explicitExperimentVariables;

        std::vector<algorithm::IterationInfo> implicitIterationsInfo;
        std::vector<algorithm::IterationInfo> explicitIterationsInfo;

        calcResults(
                implicitExperimentVariables, explicitExperimentVariables, implicitIterationsInfo,
                explicitIterationsInfo);

        if (isNeedCalculateResiduals)
        {
            calcResiduals(
                    implicitExperimentVariables, explicitExperimentVariables, implicitIterationsInfo,
                    explicitIterationsInfo);
        }

        makePlots(implicitExperimentVariables, explicitExperimentVariables, implicitIterationsInfo,
                  explicitIterationsInfo, true);
    }
    catch (std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
        utils::pauseExecution();
    }

    return 0;
}


void calcResults(
        std::vector<algorithm::Variables>& implicitExperimentVariables,
        std::vector<algorithm::Variables>& explicitExperimentVariables,
        std::vector<algorithm::IterationInfo>& implicitIterationsInfo,
        std::vector<algorithm::IterationInfo>& explicitIterationsInfo)
{
    algorithm::AlgorithmConfigurator algorithmConfigurator(
            "../res/diploma_90_grad.json", algorithm::ConfigFileTypeJson);

    algorithm::InitialParameters* initialParameters = algorithmConfigurator.readAlgorithmInitialParameters();

    std::vector<algorithm::TargetParameter>* targetParameters = algorithmConfigurator.readAlgorithmSequenceFromFile();

    algorithm::DifferenceMethod* implicitDifferenceMethod = new algorithm::ImplicitDifferenceMethod(
            &implicitExperimentVariables, &implicitIterationsInfo, initialParameters);
    algorithm::DifferenceMethod* explicitDifferenceMethod = new algorithm::ExplicitDifferenceMethod(
            &explicitExperimentVariables, &explicitIterationsInfo, initialParameters);

    implicitDifferenceMethod->setIsNeedResetTau(false);
    explicitDifferenceMethod->setIsNeedResetTau(false);

    std::function<void(long long, long long)> showIterationsProgressBarFunction =
            [](long long currentIteration, long long maxIterations)
            {
                utils::showIterationsProgressBar(currentIteration, maxIterations);
            };
    implicitDifferenceMethod->setIterationFinishedCallback(&showIterationsProgressBarFunction);
    explicitDifferenceMethod->setIterationFinishedCallback(&showIterationsProgressBarFunction);

    implicitDifferenceMethod->setTargetParameters(targetParameters);
    explicitDifferenceMethod->setTargetParameters(targetParameters);

    std::cout << "*****Algorithms info*****" << std::endl;

    std::chrono::time_point<std::chrono::steady_clock> start;
    std::chrono::time_point<std::chrono::steady_clock> end;

    start = std::chrono::steady_clock::now();

    std::future<void> implicitDifferenceMethodCalculation(std::async(
            [&implicitDifferenceMethod]()
            {
                std::chrono::time_point<std::chrono::steady_clock> start;
                std::chrono::time_point<std::chrono::steady_clock> end;

                start = std::chrono::steady_clock::now();

                std::cout << "Finite-difference method (start): " << std::endl << std::endl;

                implicitDifferenceMethod->calcResult();

                end = std::chrono::steady_clock::now();

                std::cout << "Finite-difference method (finish): " << std::endl;
                printTotalDuration(start, end);
            }));

    std::future<void> explicitDifferenceMethodCalculation(std::async(
            [&explicitDifferenceMethod]()
            {
                std::chrono::time_point<std::chrono::steady_clock> start;
                std::chrono::time_point<std::chrono::steady_clock> end;

                start = std::chrono::steady_clock::now();

                std::cout << "Tangential method (start): " << std::endl;
                explicitDifferenceMethod->calcResult();

                end = std::chrono::steady_clock::now();

                std::cout << "Tangential method (finish): " << std::endl;
                printTotalDuration(start, end);
            }));

    implicitDifferenceMethodCalculation.get();
    explicitDifferenceMethodCalculation.get();

    end = std::chrono::steady_clock::now();

    std::cout << "All calculations results: " << std::endl;
    printTotalDuration(start, end);

    delete implicitDifferenceMethod;
    delete explicitDifferenceMethod;
    delete targetParameters;
    delete initialParameters;
}


void makePlots(
        std::vector<algorithm::Variables>& implicitExperimentVariables,
        std::vector<algorithm::Variables>& explicitExperimentVariables,
        std::vector<algorithm::IterationInfo>& implicitIterationsInfo,
        std::vector<algorithm::IterationInfo>& explicitIterationsInfo,
        bool needPauseExecution)
{
    const bool isNeedMakeComparisonPlot = false;

    plot::Plot* implicitMethodPlot = configMagneticFluidPlot(
            implicitExperimentVariables, implicitIterationsInfo, "Finite-difference method",
            plot::PlotOutputTypeWindow, "");
    implicitMethodPlot->makeGraphs();

    plot::Plot* explicitMethodPlot = configMagneticFluidPlot(
            explicitExperimentVariables, explicitIterationsInfo, "Tangential method", plot::PlotOutputTypeWindow,
            "");
    explicitMethodPlot->makeGraphs();

    plot::Plot* comparisonPlot = configComparisonPlot(
            implicitExperimentVariables.back(), implicitIterationsInfo.back(), explicitExperimentVariables.back(),
            explicitIterationsInfo.back(), "Comparison", plot::PlotOutputTypeWindow, "");

    if (isNeedMakeComparisonPlot)
    {
        comparisonPlot->makeGraphs();
    }

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
        std::chrono::time_point<std::chrono::steady_clock>& start,
        std::chrono::time_point<std::chrono::steady_clock>& end)
{
    auto totalCalculationsTimeMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    auto totalCalculationsTimeSeconds = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    auto totalCalculationsTimeMinutes = std::chrono::duration_cast<std::chrono::minutes>(end - start).count();
    auto totalCalculationsTimeHours = std::chrono::duration_cast<std::chrono::hours>(end - start).count();

    std::cout << "Total calculations time: " << totalCalculationsTimeMilliseconds << " ms."
              << " ("
              << totalCalculationsTimeSeconds << " sec."
              << ", " << totalCalculationsTimeMinutes << "min."
              << ", " << totalCalculationsTimeHours << "h."
              << ")" << std::endl;
}
