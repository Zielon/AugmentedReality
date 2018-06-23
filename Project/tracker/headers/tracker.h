#ifndef PROJECT_TRACKER_H
#define PROJECT_TRACKER_H

#include <opencv2/core/mat.hpp>
#include <GLFW/glfw3.h>
#include "camera.h"

class Tracker {
public:

    Tracker();

    void findMatrix();

    void findMarker();

    void defaultSetting();

    float *getMatrix();

private:
    Camera *camera;
    cv::Mat mat;
    float matrix[16] = {};
};

#endif //PROJECT_TRACKER_H
