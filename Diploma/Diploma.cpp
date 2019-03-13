// Diploma.cpp : Defines the entry point for the application.
//

#include <sstream>
#include <iostream>
#include "Diploma.h"
#include "utils/Utils.h"
#include "plot/Plot.h"
#include "plot/Utils.h"
#include "algorithm/Algorithm.h"
#include "algorithm/EAlgorithm.h"


plot::Plot* configMagneticFluidPlot(std::vector<algorithm::Variables> &iterationVariables, 
									std::vector<algorithm::IterationInfo> &iterationsInfo);

plot::Plot* configAnglesPlot(std::vector<algorithm::Variables> &iterationVariables, 
							 std::vector<algorithm::IterationInfo> &iterationsInfo);


int main()
{
	std::vector<algorithm::Variables> experimentVariables;

	std::vector<algorithm::IterationInfo> iterationsInfo;

	algorithm::calcResult(algorithm::e::runIterationProcess, experimentVariables, iterationsInfo);

	plot::Plot *magneticFluidPlot = configMagneticFluidPlot(experimentVariables, iterationsInfo);
	magneticFluidPlot->makeGraphs();

	utils::pauseExecution();

	delete magneticFluidPlot;

	return 0;
}


plot::Plot* configMagneticFluidPlot(std::vector<algorithm::Variables> &iterationVariables, 
									std::vector<algorithm::IterationInfo> &iterationsInfo)
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
	config->title = "Magnetic fluid";
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
