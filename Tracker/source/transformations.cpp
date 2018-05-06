#include <opencv2/core/types.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv/cv.hpp>
#include "../data/subpoint.cpp"

using namespace cv;
using namespace std;

// The width length
const int Y = -1;
// The height length
const int X = -5;

class Transformations {

public:
    static int SUBMASK[5][3];

    static vector<Subpoint> getSubimage(const cv::Point2f &edge, const cv::Mat &pSrc, int contourID);

    static Mat convertToMat(std::vector<Subpoint> points);

private:
    static Point2f getPerpendicular(const cv::Point2f &p);

    static Point2f getNormalized(const cv::Point2f &p);

    static int subpixSampleSafe(const cv::Mat &pSrc, const cv::Point2f &p);
};

inline Point2f Transformations::getPerpendicular(const cv::Point2f &p) {
    auto point = Point2f();
    point.x = -p.y;
    point.y = p.x;
    return point;
}

inline Point2f Transformations::getNormalized(const cv::Point2f &p) {
    return p / norm(p);
}

inline int Transformations::subpixSampleSafe(const cv::Mat &pSrc, const cv::Point2f &p) {
    int x = int(floorf(p.x));
    int y = int(floorf(p.y));
    if (x < 0 || x >= pSrc.cols - 1 ||
        y < 0 || y >= pSrc.rows - 1)
        return 127;
    int dx = int(256 * (p.x - floorf(p.x)));
    int dy = int(256 * (p.y - floorf(p.y)));
    unsigned char *i = (pSrc.data + y * pSrc.step) + x;
    int a = i[0] + ((dx * (i[1] - i[0])) >> 8);
    i += pSrc.step;
    int b = i[0] + ((dx * (i[1] - i[0])) >> 8);
    return a + ((dy * (b - a)) >> 8);
}

inline int Transformations::SUBMASK[5][3] = {
        {-1, 2,  1},
        {-1, 1,  1},
        {-1, 0,  1},
        {-1, -2, 1},
        {-1, -3, 1}
};

inline vector<Subpoint> Transformations::getSubimage(const cv::Point2f &edge, const cv::Mat &pSrc, int contourID) {

    auto points = vector<Subpoint>();
    Point rectA, rectB;
    for (int y = Y, coord_y = 0; y < abs(Y) + 1; y++, coord_y++) {

        // Scale the edge by a small factor of the pixels length
        auto nextOnEdge = edge + (getNormalized(edge) * (double) y);
        for (int x = X, coord_x = 0; x < abs(X) + 1; x++, coord_x++) {

            // Get points from the perpendicular vector
            auto orthonormal = getNormalized(getPerpendicular(edge));
            auto nextOnOrthogonal = nextOnEdge + (orthonormal * (double) x);

            // Find pixels with the sub-pixel accuracy
            int pixelOrtho = subpixSampleSafe(pSrc, nextOnOrthogonal);
            points.emplace_back(nextOnOrthogonal, pixelOrtho, Point(coord_x, coord_y));

            // Assign two opposite points to draw a rectangle
            if (y == Y && x == X) rectA = nextOnOrthogonal;
            if (y == abs(Y) && x == abs(X)) rectB = nextOnOrthogonal;
        }
    }

    if (contourID != 5)
        rectangle(pSrc, rectA, rectB, Scalar(0, 255, 0), 1, 8, 0);
    else
        rectangle(pSrc, rectA, rectB, Scalar(255, 0, 0), 1, 8, 0);

    return points;
}

inline Mat Transformations::convertToMat(std::vector<Subpoint> points) {
    int width = abs(Y) * 2 + 1;
    int height = abs(X) * 2 + 1;
    Mat strip = Mat(Size(width, height), CV_8UC1);
    for (auto point : points)
        strip.at<uchar>(point.coords.x, point.coords.y) = (uchar) point.pixel;

    Size size(width * 10, height * 10);
    Mat dst;
    resize(strip, dst, size);
    return dst;
}

