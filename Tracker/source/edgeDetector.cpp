#include <opencv2/core/mat.hpp>
#include <opencv/cv.hpp>
#include "../headers/edgeDetector.h"
#include "../source/transformations.cpp"

using namespace cv;
using namespace std;

EdgeDetector *EdgeDetector::drawCircles() {
    int contourID = 0;
    for (auto contour : *_contours) {
        int p = 1;

        for (; p < contour.size(); p++)
            draw7points(contour[p], contour[p - 1], contourID);

        draw7points(contour[p - 1], contour[0], contourID);

        for (const auto &j : contour) {
            circle(_original, j, 1, _BLUE, 2, 8, 0);
        }
    }

    return this;
}

EdgeDetector::EdgeDetector() {
    _contours = new vector<vector<Point>>();
    _hierarchy = new vector<Vec4i>();
}

void EdgeDetector::transformToBinary() {
    cvtColor(_original, _grey, CV_BGR2GRAY);
    threshold(_grey, _binary, _threshold, 255, THRESH_BINARY);
    //	adaptiveThreshold(grayFrame, binaryFrame, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 11, 12);
    //	adaptiveThreshold(grayFrame, binaryFrame, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 11, 12);
    //  adaptiveThreshold(grayFrame, binaryFrame, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 11, 12);
}

EdgeDetector *EdgeDetector::drawEdges() {
    for (const auto &contour : *_contours)
        polylines(_original, contour, true, _RED, 2, 8, 0);

    return this;
}

void EdgeDetector::filterEdges() {
    transformToBinary();
    findContours(_binary, *_contours, *_hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    auto *filtered = new vector<vector<Point>>();
    for (const auto &contour : *_contours) {
        vector<Point> approx;
        approxPolyDP(contour, approx, 3, true);
        if (approx.size() == 4) {
            Rect rect = boundingRect(Mat(approx));
            if (rect.size().area() > 300)
                filtered->push_back(approx);
        }
    }

    _contours = filtered;
}

void EdgeDetector::draw7points(Point2f a, Point2f b, int& contourID) {
    Point2f c = b - a;
    Point2f point;
    vector<Point2f> linePoints;
    for (int i = 1; i < 8; i++) {
        double indicator = (double) i / 7.0;
        point = a + indicator * c;

        // #### EXERCISE 3 ####
        auto points = Transformations::getSubimage(point, _grey, _original, linePoints);
        auto result = Transformations::convertToMat(points);

        if(contourID == 5)
            imshow("Strip", result);
        contourID++;
    }

    fitLine(linePoints);
}

EdgeDetector *EdgeDetector::setFrame(Mat original) {
    _original = original;
    _binary = original;
    filterEdges();
    return this;
}

Mat EdgeDetector::getTransformed() {
    return _original;
}

EdgeDetector *EdgeDetector::setThreshold(int threshold) {
    _threshold = threshold;
    return this;
}

void EdgeDetector::fitLine(vector<Point2f> points) {
    Vec4f line;
    cv::fitLine(points, line, CV_DIST_L2, 0, 0.5, 0.01);
    auto d = Point2f(line[0], line[1]) * 30;
    auto s = Point2f(line[2], line[3]);
    cv::line(_original, s - d, s + d, _BLUE, 1, 8, 0);
}
