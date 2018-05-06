#include <opencv2/core/types.hpp>

using namespace cv;

struct Subpoint{
public:
    Subpoint(Point2f point2f, int pixel, Point coords);
    Point2f point;
    Point coords;
    int pixel;
};

inline Subpoint::Subpoint(Point2f point2f, int pixel, Point coords) {
    this->pixel = pixel;
    this->point = point2f;
    this->coords = coords;
}
