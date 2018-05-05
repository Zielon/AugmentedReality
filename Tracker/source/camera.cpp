#include <opencv2/videoio.hpp>
#include <opencv/cv.hpp>
#include <opencv/highgui.h>
#include <thread>
#include "../headers/camera.h"
#include "../headers/detector.h"

using namespace cv;
using namespace std;

void Camera::start() {
    int _threshold = 70;
    VideoCapture cap(_path);
    if (!cap.isOpened()) return;

    namedWindow("Threshold", 1);
    createTrackbar("Level", "Threshold", &_threshold, 255);

    bool stop = false;

    for (;;) {

        auto key = static_cast<char>(cvWaitKey(10));
        if (key == 27) break;
        if (key == 32) stop = !stop;

        if (stop)
            continue;

        if (!cap.read(_frame)) {
            // Start from the beginning
            cap.set(CV_CAP_PROP_POS_FRAMES, 0);
            continue;
        };

        Mat transformed = _detector
                ->setThreshold(_threshold)
                ->setFrame(_frame)
                        //->drawEdges()
                ->drawCircles()
                ->getTransformed();

        //imshow("Threshold", transformed);

        //std::this_thread::sleep_for(0.001s);
    }
}

Camera::Camera(std::string path, Detector *detector) {
    _path = path;
    _detector = detector;
}
