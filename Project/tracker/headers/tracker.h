#ifndef PROJECT_TRACKER_H
#define PROJECT_TRACKER_H

#include <opencv2/core/mat.hpp>
#include <GLFW/glfw3.h>
#include <thread>

#include "camera.h"

#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

class Tracker {
public:
    Tracker();

    void defaultSetting();

    Mat &getFrame();

    double *findMarker();

    Camera *getCamera();

private:
    cv::Mat frame;
    Camera *camera;
    vector<Vec3d> rotationVectors, translationVectors;

    Mat getRotationMatrix();

    Mat getQuaternion(Mat matrix);

    Vec3d rotationMatrixToEulerAngles(Mat &R);
};

#endif //PROJECT_TRACKER_H
