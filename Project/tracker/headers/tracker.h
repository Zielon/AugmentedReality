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

    std::thread findMarker();

    void defaultSetting();

    float *getMatrix();

    void setMatrix(float* matrix);

    void end(bool flag);

private:
    Camera *camera;
    cv::Mat mat;
    std::mutex mutex;
    float matrix[16];
    bool running;
};

#endif //PROJECT_TRACKER_H
