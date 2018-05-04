#ifndef TRACKER_CAMERA_H
#define TRACKER_CAMERA_H

#include <string>
#include <opencv2/core/mat.hpp>
#include "detector.h"

class Camera {
private:
    cv::Mat _frame;
    std::string _path;
    Detector *_detector;

public:
    Camera(std::string path, Detector *detector);

    void start();
};

#endif //TRACKER_CAMERA_H
