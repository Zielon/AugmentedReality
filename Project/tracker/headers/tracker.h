#ifndef PROJECT_TRACKER_H
#define PROJECT_TRACKER_H

#include <opencv2/core/mat.hpp>
#include <GLFW/glfw3.h>
#include <thread>

#include "camera.h"

class Tracker {
public:
    Tracker();

    void findMatrix();

    void findMarker();

    void defaultSetting();

    float *getMatrix();

    void setMatrix(float *matrix);

    cv::Mat mat;

private:
    Camera *camera;
    std::mutex mutex;
    float matrix[16];
};

#endif //PROJECT_TRACKER_H
