#ifndef TRACKER_EDGEDETECTOR_H
#define TRACKER_EDGEDETECTOR_H

#include <opencv2/core/mat.hpp>
#include "detector.h"

class EdgeDetector : public Detector {

private:
    cv::Mat _original;
    cv::Mat _binary;
    int _threshold;
    std::vector<std::vector<cv::Point>> *_contours;
    std::vector<cv::Vec4i> *_hierarchy;
    const cv::Scalar _RED = cv::Scalar(0, 0, 255);
    const cv::Scalar _GREEN = cv::Scalar(0, 255, 0);
    const cv::Scalar _BLUE = cv::Scalar(255, 0, 0);

    void filterEdges();

    void draw7points(cv::Point a, cv::Point b);

    void transformToBinary();

public:
    explicit EdgeDetector();

    EdgeDetector *setFrame(cv::Mat original);

    EdgeDetector *drawEdges();

    EdgeDetector *drawCircles();

    EdgeDetector *setThreshold(int threshold);

    cv::Mat getTransformed();
};

#endif //TRACKER_EGDEDETECTOR_H
