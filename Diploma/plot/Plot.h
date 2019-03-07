//
// Created by denis Belobrotski on 2019-02-26.
//

#pragma once


#define GNUPLOT             "gnuplot"


#ifdef WIN32
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
#endif

#include <vector>
#include <string>


namespace plot {

    struct Point {
        double x;
        double y;
    };

    struct Graph {
        std::vector<Point> points;
        std::string title;
    };

    struct PlotConfig {
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

        void makeGraphs() noexcept(false);

    private:
        FILE *pipe;
        PlotConfig *config;
        std::vector<Graph> *graphs;

        Plot();
    };

}
