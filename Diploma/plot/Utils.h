//
// Created by user on 2019-02-27.
//

#ifndef DIPLOMA_PLOT_UTILS_H
#define DIPLOMA_PLOT_UTILS_H

#include "Plot.h"


namespace plot
{
    void convertComponentsVectorsToPointsVector(std::vector<double> *xComponents,
                                                std::vector<double> *yComponents,
                                                std::vector<Point> *pointsVector) noexcept(false);
}

#endif //DIPLOMA_PLOT_UTILS_H
