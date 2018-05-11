#ifndef TRACKER_EDGEDETECTOR_H
#define TRACKER_EDGEDETECTOR_H

#include <opencv2/core/mat.hpp>
#include "detector.h"
#include "../data/line.h"

class EdgeDetector : public Detector {

private:
    cv::Mat _original;
    cv::Mat _grey;
    cv::Mat _binary;
    int _threshold;
    std::vector<std::vector<cv::Point>> *_contours;
    std::vector<cv::Vec4i> *_hierarchy;
    const cv::Scalar _RED = cv::Scalar(0, 0, 255);
    const cv::Scalar _GREEN = cv::Scalar(0, 255, 0);
    const cv::Scalar _BLUE = cv::Scalar(255, 0, 0);
    const cv::Scalar _WHITE = cv::Scalar(255, 255, 255);

    void filterEdges();

    Line draw7points(cv::Point2f a, cv::Point2f b, int &contourID);

    Line fitLine(std::vector<cv::Point2f> points);

    void transformToBinary();

public:
    explicit EdgeDetector();

    EdgeDetector *setFrame(cv::Mat original);

    EdgeDetector *drawEdges();

    EdgeDetector *drawCircles();

    EdgeDetector *drawMarker(std::vector<cv::Point2f> inter);

    EdgeDetector *setThreshold(int threshold);

    cv::Mat getTransformed();
};

#endif //TRACKER_EGDEDETECTOR_H
