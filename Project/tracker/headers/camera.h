#ifndef PROJECT_CAMERA_H
#define PROJECT_CAMERA_H

#include <opencv2/core/mat.hpp>
#include <opencv2/videoio.hpp>

class Camera {
public:
    Camera(int mode = 0);

    void nextFrame(cv::Mat &frame);

    ~Camera();

private:
    cv::VideoCapture *capture;
};

#endif //PROJECT_CAMERA_H
