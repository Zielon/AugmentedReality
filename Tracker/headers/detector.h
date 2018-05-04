#ifndef TRACKER_DECODER_H
#define TRACKER_DECODER_H

#include <opencv2/core/mat.hpp>

class Detector {
public:
    virtual Detector *drawCircles() = 0;

    virtual Detector *drawEdges() = 0;

    virtual Detector *setFrame(cv::Mat original) = 0;

    virtual Detector *setThreshold(int threshold) = 0;

    virtual cv::Mat getTransformed() = 0;
};

#endif //TRACKER_DECODER_H
