//
// Created by denis Belobrotski on 2019-02-26.
//

#ifndef DIPLOMA_GNUPLOTWRAPPER_H
#define DIPLOMA_GNUPLOTWRAPPER_H

#define GNUPLOT             "gnuplot"
#define GNUPLOT_WIN_WIDTH   1280
#define GNUPLOT_WIN_HEIGHT  720


#ifdef WIN32
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
#endif

#include <vector>
#include <string>

struct Point
{
    double x;
    double y;
};

struct Graph
{
    std::vector<Point> points;
    std::string title;
};

struct PlotConfig
{
    int windowWidth;
    int windowHeight;
    std::string title;
    std::string xAxisName;
    std::string yAxisName;
};

class Plot {

public:
    Plot(PlotConfig *config, std::vector<Graph> *graphs);
    ~Plot();

    void makeGraphs();

private:
    FILE *pipe;
    PlotConfig *config;
    std::vector<Graph> *graphs;

    Plot();
};


#endif //DIPLOMA_GNUPLOTWRAPPER_H
