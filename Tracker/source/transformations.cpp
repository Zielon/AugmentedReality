#include <opencv2/core/types.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv/cv.hpp>
#include "../data/subpoint.cpp"

using namespace cv;
using namespace std;

// The width length
const int WIDTH = -1;
// The height length
const int HEIGHT = -3;

class Transformations {

public:
    static int SOBEL_MASK[3][3];

    static vector<Subpoint> getSubimage(const cv::Point2f &edge, const cv::Mat &pSrc, int contourID);

    static Mat convertToMat(std::vector<Subpoint> points);

private:
    static Point2f getPerpendicular(const cv::Point2f &p);

    static Point2f getNormalized(const cv::Point2f &p);

    static int subpixelSampleSafe(const cv::Mat &pSrc, const cv::Point2f &p);
};

inline int Transformations::SOBEL_MASK[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
};

inline Point2f Transformations::getPerpendicular(const cv::Point2f &p) {
    auto point = Point2f();
    point.x = p.y;
    point.y = -p.x;
    return point;
}

inline Point2f Transformations::getNormalized(const cv::Point2f &p) {
    return p / norm(p);
}

inline int Transformations::subpixelSampleSafe(const cv::Mat &pSrc, const cv::Point2f &p) {
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

inline vector<Subpoint> Transformations::getSubimage(const cv::Point2f &edge, const cv::Mat &pSrc, int contourID) {

    auto points = vector<Subpoint>();
    Point2f rectA, rectB;
    for (int y = WIDTH, coord_y = 0; y < abs(WIDTH) + 1; y++, coord_y++) {

        // Scale the edge by a small factor of the pixels length
        auto nextOnEdge = edge + (getNormalized(edge) * (double) y);
        for (int x = HEIGHT, coord_x = 0; x < abs(HEIGHT) + 1; x++, coord_x++) {

            // Get points from the perpendicular vector
            auto orthonormal = getNormalized(getPerpendicular(edge));
            auto nextOnOrthogonal = nextOnEdge + (orthonormal * (double) x);

            // Find pixels with the sub-pixel accuracy
            int pixelOrtho = subpixelSampleSafe(pSrc, nextOnOrthogonal);
            points.emplace_back(nextOnOrthogonal, pixelOrtho, Point(coord_x, coord_y));

            // Assign two opposite points to draw a rectangle
            if (y == WIDTH && x == HEIGHT) rectA = nextOnOrthogonal;
            if (y == abs(WIDTH) && x == abs(HEIGHT)) rectB = nextOnOrthogonal;
        }
    }

    if (contourID != 5)
        rectangle(pSrc, rectA, rectB, Scalar(0, 255, 0), 1, 8, 0);
    else
        rectangle(pSrc, rectA, rectB, Scalar(255, 0, 0), 2, 8, 0);

    return points;
}

inline Mat Transformations::convertToMat(std::vector<Subpoint> points) {
    int width = abs(WIDTH) * 2 + 1;
    int height = abs(HEIGHT) * 2 + 1;
    Mat strip = Mat(Size(width, height), CV_8UC1);
    for (auto point : points)
        strip.at<uchar>(point.coords.x, point.coords.y) = (uchar) point.pixel;

    Mat dst;
    resize(strip, dst, Size(width * 10, height * 10));
    return dst;
}

