//
// Created by user on 2019-02-27.
//

#ifndef DIPLOMA_UTILS_H
#define DIPLOMA_UTILS_H

#include "Plot.h"


namespace plot
{
    void convertComponentsVectorsToPointsVector(std::vector<double> *xComponents,
                                                std::vector<double> *yComponents,
                                                std::vector<Point> *pointsVector) noexcept(false);
}

#endif //DIPLOMA_UTILS_H
