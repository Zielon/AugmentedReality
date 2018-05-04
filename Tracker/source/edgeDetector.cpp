#include <opencv2/core/mat.hpp>
#include <opencv/cv.hpp>
#include "../headers/edgeDetector.h"

using namespace cv;
using namespace std;

EdgeDetector *EdgeDetector::drawCircles() {
    for (auto contour : *_contours) {
        int p = 1;
        for (; p < contour.size(); p++)
            draw7points(contour[p], contour[p - 1]);

        draw7points(contour[p - 1], contour[0]);

        for (int j = 0; j < contour.size(); j++) {
            circle(_original, contour[j], 1, _BLUE, 2, 8, 0);
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
            if (rect.size().area() > 600)
                filtered->push_back(approx);
        }
    }

    _contours = filtered;
}

void EdgeDetector::draw7points(Point a, Point b) {
    Point c = b - a;
    for (int i = 1; i < 8; i++) {
        double indicator = (double) i / 7;
        Point point = indicator * c;
        circle(_original, a + point, 1, _GREEN, 1, 8, 0);
    }
}

EdgeDetector *EdgeDetector::setFrame(cv::Mat original) {
    _original = original;
    _binary = original;
    filterEdges();
    return this;
}

cv::Mat EdgeDetector::getTransformed() {
    return _original;
}

EdgeDetector *EdgeDetector::setThreshold(int threshold) {
    _threshold = threshold;
    return this;
}
