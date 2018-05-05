#include <opencv2/core/mat.hpp>
#include <opencv/cv.hpp>
#include "../headers/edgeDetector.h"
#include "../source/transformations.cpp"

using namespace cv;
using namespace std;

EdgeDetector *EdgeDetector::drawCircles() {
    for (auto contour : *_contours) {
        int p = 1;
        for (; p < contour.size(); p++)
            draw7points(contour[p], contour[p - 1]);

        draw7points(contour[p - 1], contour[0]);

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
    cvtColor(_original, _binary, CV_BGR2GRAY);
    threshold(_binary, _binary, _threshold, 255, THRESH_BINARY);
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

void EdgeDetector::draw7points(Point2f a, Point2f b) {
    Point2f c = b - a;
    Point2f point;
    for (int i = 1; i < 8; i++) {
        double indicator = (double) i / 7.0;
        point = a + indicator * c;
        circle(_original, point, 2, _RED, 1, 8, 0);
        Transformations::getSubimage(point, _original);
    }
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
