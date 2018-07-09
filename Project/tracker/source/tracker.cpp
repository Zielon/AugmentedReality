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
        array.assign((float *) R.datastart, (float *) R.dataend);

//        float T[16] = {
//                R.at<float>(0, 0), R.at<float>(0, 1), R.at<float>(0, 2), (float) V[0],
//                R.at<float>(1, 0), R.at<float>(1, 1), R.at<float>(1, 2), (float) V[1],
//                -R.at<float>(2, 0), -R.at<float>(2, 1), -R.at<float>(2, 2), -(float) V[2],
//                0.0, 0.0, 0.0, 1.0
//        };

        double T[16] = {
                R.at<double>(0, 0), R.at<double>(1, 0), R.at<double>(2, 0), 0.0,
                R.at<double>(0, 1), R.at<double>(1, 1), R.at<double>(2, 1), 0.0,
                R.at<double>(0, 2), R.at<double>(1, 2), R.at<double>(2, 2), 0.0,
                (double) V[0], (double) V[1], (double) V[2], 1.0
        };

        auto modelview = new double[16];

        memcpy(modelview, T, 16 * sizeof(double));

        eulerAngles = rotationMatrixToEulerAngles(R);

        if ((!isReference) && (!markerIds.empty())) {
            for (int i = 0; i < 3; i++) {
                referenceAngles[i] = eulerAngles[i];
            }
            isReference = true;
        }

        Scene::grid->setRotation(
                Vec3d(0, (eulerAngles[1] - referenceAngles[1]), -(eulerAngles[0] - referenceAngles[0])));
        Scene::grid->setOrigin(btVector3(V[0], V[1], -V[2]) * 20);
        Scene::grid->setMatrix(modelview);
        Scene::grid->update();

        return modelview;

    } catch (const std::exception &e) { return nullptr; }
}

Vec3d Tracker::rotationMatrixToEulerAngles(Mat &rotationMatrix) {

    double m00 = rotationMatrix.at<double>(0, 0);
    double m02 = rotationMatrix.at<double>(0, 2);
    double m10 = rotationMatrix.at<double>(1, 0);
    double m11 = rotationMatrix.at<double>(1, 1);
    double m12 = rotationMatrix.at<double>(1, 2);
    double m20 = rotationMatrix.at<double>(2, 0);
    double m22 = rotationMatrix.at<double>(2, 2);

    double x, y, z;

    // Assuming the angles are in radians.
    if (m10 > 0.998) { // singularity at north pole
        x = 0;
        y = CV_PI / 2;
        z = atan2(m02, m22);
    } else if (m10 < -0.998) { // singularity at south pole
        x = 0;
        y = -CV_PI / 2;
        z = atan2(m02, m22);
    } else {
        x = atan2(-m12, m11);
        y = asin(m10);
        z = atan2(-m20, m00);
    }

    cout << Vec3f(-x, y, z) << endl;

    return Vec3f(-x, y, z);
}

Mat Tracker::getRotationMatrix() {
    Mat rotationMatrix;
    if (!rotationVectors.empty())
        Rodrigues(rotationVectors[0], rotationMatrix);

    return rotationMatrix;
}

Camera *Tracker::getCamera() {
    return camera;
}

Mat Tracker::getQuaternion(Mat matrix) {
    return Mat();
}
