#include <opencv2/core/mat.hpp>
#include <opencv/cv.hpp>
#include "../headers/edgeDetector.h"
#include "../headers/transformations.h"

using namespace cv;
using namespace std;

EdgeDetector *EdgeDetector::drawCircles() {
    int contourID = 0;
    for (auto contour : *_contours) {
        int p = 1;
        vector<Line> lines;
        for (; p < contour.size(); p++)
            lines.push_back(draw7points(contour[p], contour[p - 1], contourID));

        lines.push_back(draw7points(contour[p - 1], contour[0], contourID));

        vector<Point2f> points = Transformations::getIntersection(lines, _grey);

        if (points.size() == 4) {
            for (const auto &j : points) {
                circle(_original, j, 1, _BLUE, 2, 8, 0);
            }
            drawMarker(points);
        }
        contourID++;
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

Line EdgeDetector::draw7points(Point2f a, Point2f b, int &contourID) {
    Point2f c = b - a;
    Point2f point;
    vector<Point2f> linePoints;
    for (int i = 1; i < 8; i++) {
        double indicator = (double) i / 7.0;
        point = a + indicator * c;

        // #### EXERCISE 3 ####
        auto points = Transformations::getSubimage(point, _grey, _original, linePoints);
        auto result = Transformations::convertToMat(points);

        if (contourID == 5)
            imshow("Strip", result);
        contourID++;
    }

    return fitLine(linePoints);
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

Line EdgeDetector::fitLine(vector<Point2f> points) {
    Vec4f line;
    cv::fitLine(points, line, CV_DIST_L2, 0, 0.01, 0.01);
    auto d = Point2f(line[0], line[1]) * 200;
    auto s = Point2f(line[2], line[3]);
    //cv::line(_original, s - d, s + d, _BLUE, 1, 8, 0);
    return Line(s - d, s + d);
}

EdgeDetector *EdgeDetector::drawMarker(vector<Point2f> inter) {

    vector<Point2f> points_src;
    vector<Point2f> intersections;

    // #### KEEP THE SAME ORDER ####

    sort(inter.begin(), inter.end(), [](const Point2f &p1, const Point2f &p2) { return p1.x > p2.x; });

    auto right_up = inter[0].y < inter[1].y ? inter[0] : inter[1];
    auto right_down = inter[0].y < inter[1].y ? inter[1] : inter[0];

    sort(inter.begin(), inter.end(), [](const Point2f &p1, const Point2f &p2) { return p1.x < p2.x; });

    auto left_up = inter[0].y < inter[1].y ? inter[0] : inter[1];
    auto left_down = inter[0].y < inter[1].y ? inter[1] : inter[0];

    points_src.emplace_back(Point2f(-0.5f, -0.5f)); // left_up
    points_src.emplace_back(Point2f(-0.5f, 5.5)); // left_down
    points_src.emplace_back(5.5, 5.5); // right_down
    points_src.emplace_back(Point2f(5.5, -0.5f)); // right_up*/

    intersections.emplace_back(left_up);
    intersections.emplace_back(left_down);
    intersections.emplace_back(right_down);
    intersections.emplace_back(right_up);

    // #### SOURCE ####
    auto rec = boundingRect(inter);
    auto input = Mat(_grey, rec);
    resize(input, input, Size(6 * 50, 6 * 50));

    Size size(6, 6);

    Mat transformation = getPerspectiveTransform(intersections, points_src);
    Mat marker(size, _grey.type());

    warpPerspective(_grey, marker, transformation, size);

    threshold(marker, marker, _threshold, 255, THRESH_BINARY);

    int id = 0;

    vector<vector<int>> structure;

    for(int i = 0; i < marker.rows; i++) {
        structure.emplace_back();
        for (int j = 0; j < marker.cols; j++){
            int value = marker.at<uchar>(j, i);
            structure[i].push_back(value);
        }
    }

    for(int i = 1; i < marker.rows - 1; i++) {
        for (int j = marker.cols - 1, power = 0; j > 1; j--, power++){
            int value = marker.at<uchar>(i, power);
            if(value == 255)
                id += pow(2, power);
        }
    }

    imshow("Marker source", input);

    resize(marker, marker, Size(6 * 50, 6 * 50), INTER_AREA);
    imshow("Marker", marker);

    return this;
}
