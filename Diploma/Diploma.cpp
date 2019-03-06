﻿// Diploma.cpp : Defines the entry point for the application.
//

#include <cstdio>
#include <sstream>
#include "Diploma.h"
#include "utils/Utils.h"
#include "plot/Plot.h"
#include "plot/Utils.h"
#include "algorithm/Algorithm.h"


plot::Plot* configMagneticFluidPlot(std::vector<algorithm::Variables> *iterationVariables, std::vector<int> *iterationNumbers);

plot::Plot* configAnglesPlot(std::vector<algorithm::Variables> *iterationVariables, std::vector<int> *iterationNumbers);


int main()
{
	std::vector<algorithm::Variables> iterationVariables;

	std::vector<int> iterationNumbers;

	algorithm::calcResult(iterationVariables, iterationNumbers);

	plot::Plot *magneticFluidPlot = configMagneticFluidPlot(&iterationVariables, &iterationNumbers);
	magneticFluidPlot->makeGraphs();

#ifdef _MSC_VER
    system("pause");
#else
    std::cout << "Press enter to exit...";
    std::cin.clear();
    std::cin.get();
#endif

	delete magneticFluidPlot;

	return 0;
}


plot::Plot* configMagneticFluidPlot(std::vector<algorithm::Variables> *iterationVariables, std::vector<int> *iterationNumbers)
{
	auto graphs = new std::vector<plot::Graph>();

	for (int i = 0; i < iterationNumbers->size(); i++)
	{
		int currentIteration = (*iterationNumbers)[i];

		std::vector<plot::Point> points;
		plot::convertComponentsVectorsToPointsVector(&((*iterationVariables)[currentIteration].r),
			&((*iterationVariables)[currentIteration].z),
			&points);

		std::stringstream titleStream;
		titleStream << "Iteration " << currentIteration;

		plot::Graph graph;
		graph.title = titleStream.str();
		graph.points = points;

		graphs->push_back(graph);
	}


	auto config = new plot::PlotConfig();
	config->title = "Magnetic fluid";
	config->xAxisName = "Radius";
	config->yAxisName = "Height";
	config->windowWidth = 800;
	config->windowHeight = 800;

	auto plot = new plot::Plot(config, graphs);

	return plot;
}


plot::Plot* configAnglesPlot(std::vector<algorithm::Variables> *iterationVariables, std::vector<int> *iterationNumbers)
{
	return nullptr;
}
