// Diploma.cpp : Defines the entry point for the application.
//

#include <sstream>
#include <iostream>
#include <algorithm>
#include <cmath>
#include "Diploma.h"
#include "utils/Utils.h"
#include "plot/Plot.h"
#include "plot/Utils.h"
#include "plot/Exceptions.h"
#include "algorithm/Algorithm.h"
#include "algorithm/angles/EAlgorithm.h"
#include "algorithm/difference_scheme/DSAlgorithm.h"


plot::Plot* configMagneticFluidPlot(std::vector<algorithm::Variables> &iterationVariables, 
									std::vector<algorithm::IterationInfo> &iterationsInfo,
									std::string title);

plot::Plot* configAnglesPlot(std::vector<algorithm::Variables> &iterationVariables, 
							 std::vector<algorithm::IterationInfo> &iterationsInfo);

void configPlotAxesRanges(plot::AxesRanges &axesRanges, std::vector<plot::Graph> &graphs);

plot::Plot* configComparisonPlot(std::string title,
								 algorithm::Variables &oldIterationVariables,
								 algorithm::IterationInfo &oldIterationInfo,
								 algorithm::Variables &newIterationVariables,
								 algorithm::IterationInfo &newIterationInfo);

void fillGraphTitleStreamDefault(std::stringstream &titleStream, std::string title,
								 algorithm::IterationInfo &iterationInfo);

void configDefaultPlot(plot::PlotConfig &config);

plot::Plot* createDefaultPlot(std::string title, std::vector<plot::Graph> *graphs);


int main()
{
	std::vector<algorithm::Variables> oldExperimentVariables;
	std::vector<algorithm::Variables> newExperimentVariables;

	std::vector<algorithm::IterationInfo> oldIterationsInfo;
	std::vector<algorithm::IterationInfo> newIterationsInfo;

	std::cout << "*****Algorithms info*****" << std::endl;

	std::cout << "Difference scheme: " << std::endl << std::endl;
	algorithm::calcResult(algorithm::ds::runIterationProcess, oldExperimentVariables, oldIterationsInfo);

	std::cout << "Angles: " << std::endl;
	algorithm::calcResult(algorithm::e::runIterationProcess, newExperimentVariables, newIterationsInfo);

	plot::Plot *oldMagneticFluidPlot = configMagneticFluidPlot(oldExperimentVariables, 
															   oldIterationsInfo, 
															   "Old algorithm");
	plot::Plot *newMagneticFluidPlot = configMagneticFluidPlot(newExperimentVariables,
															   newIterationsInfo,
															   "New algorithm");
	plot::Plot *comparisonPlot = configComparisonPlot("Comparison",
													  oldExperimentVariables.back(),
													  oldIterationsInfo.back(),
													  newExperimentVariables.back(),
													  newIterationsInfo.back());

	try
	{
		oldMagneticFluidPlot->makeGraphs();
		newMagneticFluidPlot->makeGraphs();
		comparisonPlot->makeGraphs();
	}
	catch (plot::PipeException &e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch (plot::VectorSizeException &e)
	{
		std::cerr << e.what() << std::endl;
	}

	std::cout << "*****Residuals*****" << std::endl;
	auto size = std::min(newIterationsInfo.size(), oldIterationsInfo.size());
	for (auto i = 0; i < size; i++)
	{
		auto currentOldIteration = oldIterationsInfo[i].index;
		auto currentNewIteration = newIterationsInfo[i].index;

		double radiusResidual = utils::calcResidual(oldExperimentVariables[currentOldIteration].r, newExperimentVariables[currentNewIteration].r);
		double heightResidual = utils::calcResidual(oldExperimentVariables[currentOldIteration].z, newExperimentVariables[currentNewIteration].z);
		double commonResidual = std::max(radiusResidual, heightResidual);

		std::cout << "Experiment number:" << std::endl;
		std::cout << "difference scheme: #" << currentOldIteration << ", angles: #" << currentNewIteration << std::endl;
		std::cout << "Residual:" << std::endl;
		std::cout << "radius: " << radiusResidual << ", height: " << heightResidual << std::endl;
		std::cout << "common: " << commonResidual << std::endl;
		std::cout << "************************" << std::endl;
	}

	utils::pauseExecution();

	delete newMagneticFluidPlot;
	delete oldMagneticFluidPlot;
	delete comparisonPlot;

	return 0;
}


plot::Plot* configMagneticFluidPlot(std::vector<algorithm::Variables> &iterationVariables, 
									std::vector<algorithm::IterationInfo> &iterationsInfo,
									std::string title)
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

	return createDefaultPlot(title, graphs);
}


plot::Plot* configAnglesPlot(std::vector<algorithm::Variables> &iterationVariables, 
							 std::vector<algorithm::IterationInfo> &iterationsInfo)
{
	return nullptr;
}


void configPlotAxesRanges(plot::AxesRanges &axesRanges, std::vector<plot::Graph> &graphs)
{

	for (auto &currentGraph : graphs)
	{
		for (auto &currentPoint : currentGraph.points)
		{
			axesRanges.xAxisRange.end = std::max(axesRanges.xAxisRange.end, currentPoint.x);
			axesRanges.yAxisRange.end = std::max(axesRanges.yAxisRange.end, currentPoint.y);
		}
	}
	axesRanges.xAxisRange.end = std::ceil(axesRanges.xAxisRange.end);
	axesRanges.yAxisRange.end = std::ceil(axesRanges.yAxisRange.end);
}


plot::Plot* configComparisonPlot(std::string title, 
								 algorithm::Variables &oldIterationVariables,
								 algorithm::IterationInfo &oldIterationInfo, 
								 algorithm::Variables &newIterationVariables,
								 algorithm::IterationInfo &newIterationInfo)
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

	return createDefaultPlot(title, graphs);
}


void fillGraphTitleStreamDefault(std::stringstream &titleStream, std::string title, 
								 algorithm::IterationInfo &iterationInfo)
{
	titleStream << title << " - "
				<< "TAU: " << iterationInfo.tau
				<< ", U: " << iterationInfo.u
				<< ", B0: " << iterationInfo.b0
				<< ", A1: " << iterationInfo.a1
				<< ", A2: " << iterationInfo.a2;
}


void configDefaultPlot(plot::PlotConfig &config)
{
	config.windowWidth = 1600;
	config.windowHeight = 900;
	config.xAxisName = "Radius";
	config.yAxisName = "Height";
	config.legendFontSize = 8;
	config.equalAxes = true;
}


plot::Plot* createDefaultPlot(std::string title, std::vector<plot::Graph> *graphs)
{
	plot::AxesRanges axesRanges{ plot::Range(1, 0), plot::Range(0, 0) };
	auto config = new plot::PlotConfig();
	configPlotAxesRanges(axesRanges, *graphs);
	configDefaultPlot(*config);
	config->title = title;
	config->axesRanges = axesRanges;

	return new plot::Plot(config, graphs);
}
