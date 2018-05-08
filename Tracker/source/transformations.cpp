#include <opencv2/core/types.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv/cv.hpp>
#include "../data/subpoint.cpp"

using namespace cv;
using namespace std;

// The width length
const int WIDTH = -10;
// The height length
const int HEIGHT = -15;

class Transformations {

public:
    static double SOBEL_MASK_X[3][3];
    static double SOBEL_MASK_Y[3][3];

    static vector<vector<Subpoint>> getSubimage(const cv::Point2f &edge, cv::Mat &binary, cv::Mat &original, int contourID);

    static Mat convertToMat(vector<vector<Subpoint>> points);

private:
    static Point2f getPerpendicular(const cv::Point2f &p);

    static Point2f getNormalized(const cv::Point2f &p);

    static int subpixelSampleSafe(const cv::Mat &pSrc, const cv::Point2f &p);

    static double sobelFilter(double maskX[][3], double maskY[][3], Mat &image);
};

inline double Transformations::SOBEL_MASK_X[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
};

inline double Transformations::SOBEL_MASK_Y[3][3] = {
        {-1, -2, -1},
        {0,  0,  0},
        {1,  2,  1}
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

inline vector<vector<Subpoint>> Transformations::getSubimage(const cv::Point2f &edge, cv::Mat &binary, cv::Mat &original, int contourID) {

    auto points = vector<vector<Subpoint>>();
    Point2f rectA, rectB;
    for (int y = WIDTH, coord_y = 0; y < abs(WIDTH) + 1; y++, coord_y++) {

        points.emplace_back();

        // Scale the edge by a small factor of the pixels length
        auto nextOnEdge = edge + (getNormalized(edge) * (double) y);
        for (int x = HEIGHT; x < abs(HEIGHT) + 1; x++) {

            // Get points from the perpendicular vector
            auto orthonormal = getNormalized(getPerpendicular(edge));
            auto nextOnOrthogonal = nextOnEdge + (orthonormal * (double) x);

            // Find pixels with the sub-pixel accuracy
            int pixelOrtho = subpixelSampleSafe(binary, nextOnOrthogonal);
            points[coord_y].emplace_back(nextOnOrthogonal, pixelOrtho);

            // Assign two opposite points to draw a rectangle
            if (y == WIDTH && x == HEIGHT) rectA = nextOnOrthogonal;
            if (y == abs(WIDTH) && x == abs(HEIGHT)) rectB = nextOnOrthogonal;
        }
    }

/*
    if (contourID != 5)
        rectangle(original, rectA, rectB, Scalar(0, 255, 0), 1, 8, 0);
    else
        rectangle(original, rectA, rectB, Scalar(255, 0, 0), 2, 8, 0);
*/

    if (contourID == 5)
        rectangle(original, rectA, rectB, Scalar(0, 255, 0), 2, 8, 0);

    return points;
}


inline Mat Transformations::convertToMat(vector<vector<Subpoint>> points) {
    int width = abs(WIDTH) * 2 + 1;
    int height = abs(HEIGHT) * 2 + 1;
    Mat strip = Mat(Size(width, height), CV_8UC1);

    for(int x = 0; x < width; x++)
        for(int y = 0; y < height; y++)
            strip.at<uchar>(y, x) = (uchar) points[x][y].pixel;

    Mat dst;
    //sobelFilter(SOBEL_MASK_X, SOBEL_MASK_Y, strip);
    resize(strip, dst, Size(width * 10, height * 10));
    return dst;
}

inline double Transformations::sobelFilter(double maskX[][3], double maskY[][3], Mat &image) {

    for(int y = 1; y < image.rows - 1; y++){
        for(int x = 1; x < image.cols - 1; x++){

            double weightX = 0.0, weightY = 0.0;

            for (int a = 0; a < 3; a++) {
                for (int b = 0; b < 3; b++) {
                    int xn = x + b - 1;
                    int yn = y + a - 1;
                    weightX += image.at<uchar>(yn, xn) * maskX[a][b];
                    weightY += image.at<uchar>(yn, xn) * maskY[a][b];
                }
            }

            double sum = abs(weightX) + abs(weightY);
            sum = sum > 255 ? 255 : sum;
            sum = sum < 0 ? 0 : sum;
            image.at<uchar>(y, x) = static_cast<const uchar>(sum);
        }
    }
}

