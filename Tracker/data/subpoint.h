#ifndef SUBPOINT_H
#define SUBPOINT_H

#include <opencv2/core/types.hpp>

using namespace cv;

struct Subpoint {
public:
    Subpoint(Point2f point2f, int pixel);

    Point2f point;
    int pixel;
};

inline Subpoint::Subpoint(Point2f point2f, int pixel) {
    this->pixel = pixel;
    this->point = point2f;
}

#endif