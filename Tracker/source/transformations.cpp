#include <opencv2/core/types.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv/cv.hpp>
#include "../data/subpoint.cpp"

using namespace cv;
using namespace std;

class Transformations {

public:
    static int SUBMASK[5][3];

    static vector<Subpoint> getSubimage(const cv::Point2f &edge, const cv::Mat &pSrc);

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

inline vector<Subpoint> Transformations::getSubimage(const cv::Point2f &edge, const cv::Mat &pSrc) {
    auto points = vector<Subpoint>();
    for (auto &y : {-3, -2, -1, 0, 1, 2, 3}) {
        auto nextOnEdge = edge + (getNormalized(edge) * y);
        for (auto &x: {-1, 0, 1}) {
            auto orthonormal = getNormalized(getPerpendicular(nextOnEdge));
            auto nextOnOrthogonal = nextOnEdge + (orthonormal * x);

            int pixelOrtho = subpixSampleSafe(pSrc, nextOnOrthogonal);
            points.emplace_back(nextOnOrthogonal, pixelOrtho);
        }
    }

    auto rect = vector<Point>();

    for (int i = 0; i < points.size(); i++)
        rect.push_back(points[i].point);

    polylines(pSrc, rect, true, Scalar(0, 255, 0), 1, 8, 0);

    imshow("Threshold", pSrc);

    return points;
}

