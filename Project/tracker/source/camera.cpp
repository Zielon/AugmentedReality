#include <opencv2/videoio/videoio_c.h>

#include "../headers/camera.h"

using namespace cv;
using namespace std;

Camera::Camera(int mode) {
    switch (mode) {
        case 0:
            this->capture->open(0);             //how to open camera with this videoCapture pointer??
            break;
        case 1:
            this->capture = new VideoCapture("resources/movie");
        default:
            break;
    }
}

void Camera::nextFrame(cv::Mat &frame) {
    if (!capture->isOpened()) return;

    if (!capture->read(frame)) {
        capture->set(CV_CAP_PROP_POS_FRAMES, 0);
        capture->read(frame);
    };
}

Camera::~Camera() {
    this->capture->release();
    delete capture;
}