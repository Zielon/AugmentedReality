#include <opencv2/videoio/videoio_c.h>
#include "../headers/camera.h"

using namespace cv;

Camera::Camera() {
    this->capture = VideoCapture("resources/movie");
}

void Camera::nextFrame(cv::Mat &frame) {
    if (!capture.isOpened()) return;
    if (!capture.read(frame)) {
        capture.set(CV_CAP_PROP_POS_FRAMES, 0);
    };
}