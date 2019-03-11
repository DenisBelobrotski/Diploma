//
// Created by Denis Belobrotski on 2019-02-26.
//

#include <sstream>

#include "Plot.h"
#include "Exceptions.h"


plot::Plot::Plot()
{
	pipe = nullptr;
	config = nullptr;
	graphs = nullptr;
}


plot::Plot::Plot(PlotConfig *config, std::vector<Graph> *graphs)
{
#ifdef WIN32
    pipe = _popen(GNUPLOT, "w");
#else
    pipe = popen(GNUPLOT, "w");
#endif
    this->config = config;
    this->graphs = graphs;
}


plot::Plot::~Plot()
{
	delete this->config;
	delete this->graphs;

#ifdef WIN32
    _pclose(pipe);
#else
    pclose(pipe);
#endif
}


void plot::Plot::configPlot()
{
#ifdef WIN32
	fprintf(pipe, "set term wxt size %d, %d\n", config->windowWidth, config->windowHeight);
#else
	fprintf(pipe, "set term qt size %d, %d\n", config->windowWidth, config->windowHeight);
#endif
	fprintf(pipe, "set title \"%s\"\n", config->title.c_str());
	fprintf(pipe, "set xlabel \"%s\"\n", config->xAxisName.c_str());
	fprintf(pipe, "set ylabel \"%s\"\n", config->yAxisName.c_str());
	if (config->equalAxes)
	{
		fprintf(pipe, "set size ratio -1\n");
	}
}


void plot::Plot::makeGraphs() noexcept(false)
{
    if (pipe == nullptr)
    {
        throw exceptions::PipeException("Gnuplot pipe wasn't opened");
    }

	configPlot();

	auto graphsNum = graphs->size();

	fprintf(pipe, "plot '-' with lines title '%s'", (*graphs)[0].title.c_str());
	for (auto i = 1; i < graphsNum; i++)
	{
		fprintf(pipe, ", '-' with lines title '%s'", (*graphs)[i].title.c_str());
	}
	fprintf(pipe, "\n");

	for (auto& currentGraph : *graphs)
	{
		for (auto& currentPoint : currentGraph.points)
		{
			fprintf(pipe, "%lf %lf\n", currentPoint.x, currentPoint.y);
		}

		fprintf(pipe, "e\n");
		fflush(pipe);
	}
}
