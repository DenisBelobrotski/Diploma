// Diploma.cpp : Defines the entry point for the application.
//

#include <sstream>
#include <iostream>
#include <algorithm>
#include "Diploma.h"
#include "utils/Utils.h"
#include "plot/Plot.h"
#include "plot/Utils.h"
#include "algorithm/Algorithm.h"
#include "algorithm/angles/EAlgorithm.h"
#include "algorithm/difference_scheme/DSAlgorithm.h"


plot::Plot* configMagneticFluidPlot(std::vector<algorithm::Variables> &iterationVariables, 
									std::vector<algorithm::IterationInfo> &iterationsInfo,
									std::string title);

plot::Plot* configAnglesPlot(std::vector<algorithm::Variables> &iterationVariables, 
							 std::vector<algorithm::IterationInfo> &iterationsInfo);


int main()
{
	std::vector<algorithm::Variables> oldExperimentVariables;
	std::vector<algorithm::Variables> newExperimentVariables;

	std::vector<algorithm::IterationInfo> oldIterationsInfo;
	std::vector<algorithm::IterationInfo> newIterationsInfo;

	std::cout << "*****Algorithms info*****" << std::endl;

	std::cout << "Difference scheme: " << std::endl;
	algorithm::calcResult(algorithm::ds::runIterationProcess, oldExperimentVariables, oldIterationsInfo);

	std::cout << "Angles: " << std::endl;
	algorithm::calcResult(algorithm::e::runIterationProcess, newExperimentVariables, newIterationsInfo);

	plot::Plot *oldMagneticFluidPlot = configMagneticFluidPlot(oldExperimentVariables, oldIterationsInfo, "Old algorithm");
	oldMagneticFluidPlot->makeGraphs();

	plot::Plot *newMagneticFluidPlot = configMagneticFluidPlot(newExperimentVariables, newIterationsInfo, "New algorithm");
	newMagneticFluidPlot->makeGraphs();

	std::cout << "*****Residuals*****" << std::endl;
	auto size = std::min(newIterationsInfo.size(), oldIterationsInfo.size());
	for (int i = 0; i < size; i++)
	{
		auto currentOldIteration = oldIterationsInfo[i].index;
		auto currentNewIteration = newIterationsInfo[i].index;

		double radiusResidual = utils::calcResidual(oldExperimentVariables[currentOldIteration].r, newExperimentVariables[currentNewIteration].r);
		double heightResidual = utils::calcResidual(oldExperimentVariables[currentOldIteration].z, newExperimentVariables[currentNewIteration].z);
		double commonResidual = std::max(radiusResidual, heightResidual);

		std::cout << "Experiment number:" << std::endl;
		std::cout << "difference sheme: #" << currentOldIteration << ", angles: #" << currentNewIteration << std::endl;
		std::cout << "Residual:" << std::endl;
		std::cout << "radius: " << radiusResidual << ", height: " << heightResidual << std::endl;
		std::cout << "common: " << commonResidual << std::endl;
		std::cout << "************************" << std::endl;
	}

	utils::pauseExecution();

	delete newMagneticFluidPlot;
	delete oldMagneticFluidPlot;

	return 0;
}


plot::Plot* configMagneticFluidPlot(std::vector<algorithm::Variables> &iterationVariables, 
									std::vector<algorithm::IterationInfo> &iterationsInfo,
									std::string title)
{
	auto graphs = new std::vector<plot::Graph>();

	for (int i = 0; i < iterationsInfo.size(); i++)
	{
		long long currentIteration = iterationsInfo[i].index;

		std::vector<plot::Point> points;
		plot::convertComponentsVectorsToPointsVector(iterationVariables[currentIteration].r,
			iterationVariables[currentIteration].z,
			points);

		std::stringstream titleStream;
		titleStream << "#" << currentIteration << " - "
					<< "U: " << iterationsInfo[i].u
					<< ", B0: " << iterationsInfo[i].b0
					<< ", A1: " << iterationsInfo[i].a1
					<< ", A2: " << iterationsInfo[i].a2;

		plot::Graph graph;
		graph.title = titleStream.str();
		graph.points = points;

		graphs->push_back(graph);
	}


	auto config = new plot::PlotConfig();
	config->windowWidth = 1600;
	config->windowHeight = 900;
	config->title = title;
	config->xAxisName = "Radius";
	config->yAxisName = "Height";
	config->legendFontSize = 0;
	config->xAxisRange = plot::Range(1, 1);
	config->yAxisRange = plot::Range(0, 2);
	config->equalAxes = true;

	auto plot = new plot::Plot(config, graphs);

	return plot;
}


plot::Plot* configAnglesPlot(std::vector<algorithm::Variables> &iterationVariables, 
							 std::vector<algorithm::IterationInfo> &iterationsInfo)
{
	return nullptr;
}
