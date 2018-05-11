#ifndef LINE_H
#define LINE_H

#include <opencv2/core/types.hpp>

using namespace cv;

struct Line {
public:
    Line(Point2f a, Point2f b);

    Point2f a;
    Point2f b;
};

inline Line::Line(Point2f a, Point2f b) {
    this->a = a;
    this->b = b;
}

#endif