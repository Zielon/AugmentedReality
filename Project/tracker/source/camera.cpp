#include <opencv2/videoio/videoio_c.h>
#include <iostream>
#include <thread>
#include "../headers/camera.h"

using namespace cv;
using namespace std;


Camera::Camera() {
    this->capture = new VideoCapture("resources/movie");
}

void Camera::nextFrame(cv::Mat &frame) {
    cout << "nextFrame " << this_thread::get_id() << endl;
    if (!capture->isOpened()) return;
    if (!capture->read(frame)) {
        capture->set(CV_CAP_PROP_POS_FRAMES, 0);
    };
}

Camera::~Camera() {
    this->capture->release();
    delete capture;
}
