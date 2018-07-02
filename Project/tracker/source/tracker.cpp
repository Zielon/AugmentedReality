#include <opencv/highgui.h>
#include <opencv2/aruco.hpp>
#include <opencv/cv.hpp>
#include "../headers/tracker.h"
#include "../../bounce/headers/application.h"

using namespace cv;
using namespace std;

Tracker::Tracker() {
    this->camera = new Camera(1);
}

void Tracker::defaultSetting() {}

Mat &Tracker::getFrame() {
    return frame;
}

double *Tracker::findMarker() {
    camera->nextFrame(frame);

    vector<int> markerIds;
    vector<vector<Point2f>> markerCorners, rejectedCandidates;
    Ptr<aruco::Dictionary> dictionary = aruco::getPredefinedDictionary(aruco::DICT_4X4_1000);

    aruco::detectMarkers(frame, dictionary, markerCorners, markerIds);
    aruco::estimatePoseSingleMarkers(markerCorners,
                                     camera->getSquareDimension(),
                                     camera->getCameraMatrix(),
                                     camera->getDistortionCoefficient(),
                                     rotationVectors, translationVectors);

    if (markerIds.empty()) return nullptr;

    try {

        aruco::drawAxis(frame,
                        camera->getCameraMatrix(),
                        camera->getDistortionCoefficient(),
                        rotationVectors,
                        translationVectors,
                        0.05);

        auto R = getRotationMatrix();
        auto V = translationVectors[0];

        std::vector<float> array;
        array.assign((float*)R.datastart, (float*)R.dataend);

//        float T[16] = {
//                R.at<float>(0, 0), R.at<float>(0, 1), R.at<float>(0, 2), (float) V[0],
//                R.at<float>(1, 0), R.at<float>(1, 1), R.at<float>(1, 2), (float) V[1],
//                -R.at<float>(2, 0), -R.at<float>(2, 1), -R.at<float>(2, 2), -(float) V[2],
//                0.0, 0.0, 0.0, 1.0
//        };

        double T[16] = {
                R.at<float>(0, 0), R.at<float>(1, 0), R.at<float>(2, 0), 0.0,
                R.at<float>(0, 1), R.at<float>(1, 1), R.at<float>(2, 1), 0.0,
                R.at<float>(0, 2), R.at<float>(1, 2), R.at<float>(2, 2), 0.0,
                (float) V[0], (float) V[1], (float) V[2], 1.0
        };

        auto modelview = new double[16];

        memcpy(modelview, T, 16 * sizeof(double));

        Scene::grid->setRotation(rotationVectors[0]);
        Scene::grid->setMatrix((btScalar*)modelview);

        return modelview;

    } catch (const std::exception &e) { return nullptr; }
}

Mat Tracker::getRotationMatrix() {
    Mat rotationMatrix;
    if (!rotationVectors.empty())
        Rodrigues(rotationVectors[0], rotationMatrix);

    Mat matrix;
    rotationMatrix.convertTo(matrix, CV_32FC1);
    return matrix;
}

Camera *Tracker::getCamera() {
    return camera;
}

Mat Tracker::getQuaternion(Mat matrix) {
    return Mat();
}
