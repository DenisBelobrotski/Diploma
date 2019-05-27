#include "Diploma.h"
#include "Utils.h"
#include "DiplomaPlotHelper.h"
#include "Configurator.h"

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


namespace diploma
{
    const auto CONFIG_FILE_PATH = "../res/diploma_90_grad_uniform.json";

    void calcResults(
            std::vector<algorithm::IterationInfo>& firstExperimentIterationsInfo,
            std::vector<algorithm::IterationInfo>& secondExperimentIterationsInfo,
            Configurator& configurator);

    void makePlots(
            std::vector<algorithm::IterationInfo>& firstExperimentIterationsInfo,
            std::vector<algorithm::IterationInfo>& secondExperimentIterationsInfo,
            Configurator& configurator);

    void calcResiduals(
            std::vector<algorithm::IterationInfo>& firstExperimentIterationsInfo,
            std::vector<algorithm::IterationInfo>& secondExperimentIterationsInfo);

    void printTotalDuration(
            std::chrono::time_point<std::chrono::steady_clock>& start,
            std::chrono::time_point<std::chrono::steady_clock>& end);
}


int main()
{
    try
    {
        diploma::Configurator diplomaConfigurator(diploma::CONFIG_FILE_PATH, diploma::ConfigFileTypeJson);

        std::vector<algorithm::IterationInfo> implicitIterationsInfo;
        std::vector<algorithm::IterationInfo> explicitIterationsInfo;

        diploma::calcResults(implicitIterationsInfo, explicitIterationsInfo, diplomaConfigurator);

        if (diplomaConfigurator.isNeedCalculateResiduals())
        {
            diploma::calcResiduals(implicitIterationsInfo, explicitIterationsInfo);
        }

        diploma::makePlots(implicitIterationsInfo, explicitIterationsInfo, diplomaConfigurator);
    }
    catch (std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
        diploma::pauseExecution();
    }

    return 0;
}


void diploma::calcResults(
        std::vector<algorithm::IterationInfo>& firstExperimentIterationsInfo,
        std::vector<algorithm::IterationInfo>& secondExperimentIterationsInfo,
        Configurator& configurator)
{
    algorithm::AlgorithmConfigurator algorithmConfigurator(diploma::CONFIG_FILE_PATH, algorithm::ConfigFileTypeJson);

    algorithm::InitialParameters* initialParameters = algorithmConfigurator.readAlgorithmInitialParameters();

    std::vector<algorithm::TargetParameter>* targetParameters = algorithmConfigurator.readAlgorithmSequenceFromFile();

    algorithm::DifferenceMethod* firstDifferenceMethod = nullptr;
    algorithm::DifferenceMethod* secondDifferenceMethod = nullptr;

    if (configurator.isComparisonUniformNonUniform())
    {
        firstDifferenceMethod =
                new algorithm::ImplicitDifferenceMethod(&firstExperimentIterationsInfo, initialParameters);
        secondDifferenceMethod =
                new algorithm::ImplicitMethodUniformConcentration(&secondExperimentIterationsInfo, initialParameters);
    }
    else if (configurator.isNonUniformConcentration())
    {
        firstDifferenceMethod =
                new algorithm::ImplicitDifferenceMethod(&firstExperimentIterationsInfo, initialParameters);
        secondDifferenceMethod =
                new algorithm::ExplicitDifferenceMethod(&secondExperimentIterationsInfo, initialParameters);
    }
    else
    {
        firstDifferenceMethod =
                new algorithm::ImplicitMethodUniformConcentration(&firstExperimentIterationsInfo, initialParameters);
        secondDifferenceMethod =
                new algorithm::ExplicitMethodUniformConcentration(&secondExperimentIterationsInfo, initialParameters);
    }

    firstDifferenceMethod->setIsNeedResetTau(false);
    secondDifferenceMethod->setIsNeedResetTau(false);

    std::function<void(long long, long long)> showIterationsProgressBarFunction =
            [](long long currentIteration, long long maxIterations)
            {
                diploma::showIterationsProgressBar(currentIteration, maxIterations);
            };
    firstDifferenceMethod->setIterationFinishedCallback(&showIterationsProgressBarFunction);
    secondDifferenceMethod->setIterationFinishedCallback(&showIterationsProgressBarFunction);

    firstDifferenceMethod->setTargetParameters(targetParameters);
    secondDifferenceMethod->setTargetParameters(targetParameters);

    std::cout << "*****Algorithms info*****" << std::endl;

    std::chrono::time_point<std::chrono::steady_clock> start;
    std::chrono::time_point<std::chrono::steady_clock> end;

    start = std::chrono::steady_clock::now();

    std::future<void> implicitDifferenceMethodCalculation(std::async(
            [&firstDifferenceMethod]()
            {
                std::chrono::time_point<std::chrono::steady_clock> start;
                std::chrono::time_point<std::chrono::steady_clock> end;

                start = std::chrono::steady_clock::now();

                std::cout << "First experiment (start): " << std::endl << std::endl;

                firstDifferenceMethod->calcResult();

                end = std::chrono::steady_clock::now();

                std::cout << "First experiment (finish): " << std::endl;
                printTotalDuration(start, end);
            }));

    std::future<void> explicitDifferenceMethodCalculation(std::async(
            [&secondDifferenceMethod]()
            {
                std::chrono::time_point<std::chrono::steady_clock> start;
                std::chrono::time_point<std::chrono::steady_clock> end;

                start = std::chrono::steady_clock::now();

                std::cout << "Second experiment (start): " << std::endl;
                secondDifferenceMethod->calcResult();

                end = std::chrono::steady_clock::now();

                std::cout << "Second experiment (finish): " << std::endl;
                printTotalDuration(start, end);
            }));

    implicitDifferenceMethodCalculation.get();
    explicitDifferenceMethodCalculation.get();

    end = std::chrono::steady_clock::now();

    std::cout << "All calculations results: " << std::endl;
    printTotalDuration(start, end);

    delete firstDifferenceMethod;
    delete secondDifferenceMethod;
    delete targetParameters;
    delete initialParameters;
}


void diploma::makePlots(
        std::vector<algorithm::IterationInfo>& firstExperimentIterationsInfo,
        std::vector<algorithm::IterationInfo>& secondExperimentIterationsInfo,
        Configurator& configurator)
{
    plot::Plot* firstExperimentPlot =
            configMagneticFluidPlot(
                    firstExperimentIterationsInfo, "First experiment", plot::PlotOutputTypeWindow, "");
    firstExperimentPlot->makeGraphs();

    plot::Plot* secondExperimentPlot =
            configMagneticFluidPlot(
                    secondExperimentIterationsInfo, "Second experiment", plot::PlotOutputTypeWindow, "");
    secondExperimentPlot->makeGraphs();

    plot::Plot* comparisonPlot =
            configComparisonPlot(
                    firstExperimentIterationsInfo.back(), secondExperimentIterationsInfo.back(), "Comparison",
                    plot::PlotOutputTypeWindow, "");

    if (configurator.isNeedMakeComparisonPlot())
    {
        comparisonPlot->makeGraphs();
    }

    if (configurator.isNeedPauseExecutionBeforeTermination())
    {
        diploma::pauseExecution();
    }

    delete secondExperimentPlot;
    delete firstExperimentPlot;
    delete comparisonPlot;
}


void diploma::calcResiduals(
        std::vector<algorithm::IterationInfo>& firstExperimentIterationsInfo,
        std::vector<algorithm::IterationInfo>& secondExperimentIterationsInfo)
{
    std::cout << "*****Residuals*****" << std::endl;
    auto size = std::min(secondExperimentIterationsInfo.size(), firstExperimentIterationsInfo.size());
    for (auto i = 0; i < size; i++)
    {
        auto& currentFirstExperimentIterationInfo = firstExperimentIterationsInfo[i];
        auto& currentSecondExperimentIterationInfo = secondExperimentIterationsInfo[i];

        double radiusResidual = algorithm::calcResidual(currentFirstExperimentIterationInfo.variables.r,
                                                        currentSecondExperimentIterationInfo.variables.r);
        double heightResidual = algorithm::calcResidual(currentFirstExperimentIterationInfo.variables.z,
                                                        currentSecondExperimentIterationInfo.variables.z);
        double commonResidual = std::max(radiusResidual, heightResidual);

        std::cout << "Experiment number:" << std::endl;
        std::cout << "First experiment: #" << currentFirstExperimentIterationInfo.index << std::endl;
        std::cout << "Second Experiment: #" << currentSecondExperimentIterationInfo.index << std::endl;
        std::cout << "Residual:" << std::endl;
        std::cout << "radius: " << radiusResidual << ", height: " << heightResidual << std::endl;
        std::cout << "common: " << commonResidual << std::endl;
        std::cout << "************************" << std::endl;
    }
}


void diploma::printTotalDuration(
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
