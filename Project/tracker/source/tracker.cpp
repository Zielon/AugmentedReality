#include <opencv/highgui.h>
#include <opencv2/aruco.hpp>
#include "../headers/tracker.h"
#include "../source/PoseEstimation.cpp"
#include "../../bounce/headers/drawing.h"
#include "../../bounce/headers/application.h"

using namespace cv;
using namespace std;

float SIGN(float x);

float NORM(float a, float b, float c, float d);

Mat mRot2Quat(const Mat &m);

Mat Tracker::cameraMatrix = Mat::eye(3, 3, CV_64F);
Mat Tracker::distanceCoeffients = Mat();

Tracker::Tracker() {
    this->camera = new Camera(1);
}

void Tracker::defaultSetting() {
    loadCameraCalibration("resources/CameraMatrix", cameraMatrix, distanceCoeffients);
}

Mat &Tracker::getFrame() {
    return frame;
}

void Tracker::buildProjectionMatrix(double *projectionMatrix) {

    // Clipping
    float near = 0.01f;
    float far = 100.0f;

    float width = Application::WIDTH;
    float height = Application::HEIGHT;

    // Camera parameters
    double fx = cameraMatrix.at<double>(0, 0);
    double fy = cameraMatrix.at<double>(1, 1);
    double cx = cameraMatrix.at<double>(0, 2);
    double cy = cameraMatrix.at<double>(1, 2);

    projectionMatrix[0] = 2.0f * fx / width;
    projectionMatrix[1] = 0.0f;
    projectionMatrix[2] = 0.0f;
    projectionMatrix[3] = 0.0f;

    projectionMatrix[4] = 0.0f;
    projectionMatrix[5] = 2.0f * fy / height;
    projectionMatrix[6] = 0.0f;
    projectionMatrix[7] = 0.0f;

    projectionMatrix[8] = (width - 2 * cx) / width;
    projectionMatrix[9] = (height - 2 * cy) / height;
    projectionMatrix[10] = (-far - near) / (far - near);
    projectionMatrix[11] = -1.0f;

    projectionMatrix[12] = 0.0f;
    projectionMatrix[13] = 0.0f;
    projectionMatrix[14] = -2.0f * far * near / (far - near);
    projectionMatrix[15] = 0.0f;
}

float *Tracker::findMarker() {
    camera->nextFrame(frame);

    vector<int> markerIds;
    vector<vector<Point2f>> markerCorners, rejectedCandidates;
    Ptr<aruco::Dictionary> dictionary = aruco::getPredefinedDictionary(aruco::DICT_4X4_1000);

    aruco::detectMarkers(frame, dictionary, markerCorners, markerIds);
    aruco::estimatePoseSingleMarkers(markerCorners, arucoSquareDimension, cameraMatrix, distanceCoeffients,
                                     rotationVectors, translationVectors);

    if (markerIds.empty()) return nullptr;

    try {
        aruco::drawAxis(frame, cameraMatrix, distanceCoeffients, rotationVectors, translationVectors, 0.05);

        float length = 0.5;

        vector<Point3f> axisPoints;
        axisPoints.push_back(Point3f(0, 0, 0));
        axisPoints.push_back(Point3f(length, 0, 0));
        axisPoints.push_back(Point3f(0, length, 0));
        axisPoints.push_back(Point3f(0, 0, length));
        vector<Point2f> imagePoints;

        projectPoints(axisPoints, rotationVectors, translationVectors, cameraMatrix, distanceCoeffients, imagePoints);

        auto R = getRotationMatrix();
        auto V = translationVectors[0];

        float T[16] = {
                R.at<float>(0, 0), R.at<float>(0, 1), R.at<float>(0, 2), (float) V[0],
                R.at<float>(1, 0), R.at<float>(1, 1), R.at<float>(1, 2), (float) V[1],
                -R.at<float>(2, 0), -R.at<float>(2, 1), -R.at<float>(2, 2), -(float) V[2],
                0.0, 0.0, 0.0, 1.0
        };

//        float T[16] = {
//                R.at<float>(0, 0), R.at<float>(1, 0), R.at<float>(2, 0), 0.0,
//                R.at<float>(0, 1), R.at<float>(1, 1), R.at<float>(2, 1), 0.0,
//                R.at<float>(0, 2), R.at<float>(1, 2), R.at<float>(2, 2), 0.0,
//                (float) V[0], (float) V[1], (float) V[2], 1.0
//        };

        auto corners = markerCorners[0].data();

        double projectionMatrix[16];

        buildProjectionMatrix(projectionMatrix);

        //estimateSquarePose(T, corners, 0.1);

        if (frame.rows == 0 || frame.cols == 0) return nullptr;

        unsigned char pixels[Application::HEIGHT * Application::WIDTH * 3];

        memcpy(pixels, frame.data, sizeof(pixels));

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glViewport(0, 0, (GLsizei) Application::WIDTH, (GLsizei) Application::HEIGHT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glMultMatrixd(projectionMatrix);

        glDisable(GL_DEPTH_TEST);

        glRasterPos2i(0, frame.cols - 1);
        glDrawPixels(Application::WIDTH, Application::HEIGHT, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);

        glEnable(GL_DEPTH_TEST);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        //glMultMatrixf(T);

        glTranslatef(0, 0, -5);

        return T;

    } catch (const std::exception &e) { return nullptr; }
}

float *Tracker::getMatrix() {
    return this->matrix;
}

Mat Tracker::getRotationMatrix() {
    Mat rotationMatrix;
    if (rotationVectors.size() != 0)
        Rodrigues(rotationVectors[0], rotationMatrix);

    return rotationMatrix;
}

Mat Tracker::getQuaternion(Mat matrix) {
    Mat quat = mRot2Quat(matrix);

    return quat;
}

void Tracker::createBoardPosition(Size boardSize, float squareEdgeLength, vector<Point3f> &corners) {
    for (int i = 0; i < boardSize.height; i++) {
        for (int j = 0; j < boardSize.width; j++) {
            corners.push_back(Point3f(j * squareEdgeLength, i * squareEdgeLength, 0.0f));
        }
    }
}

void Tracker::getChessboardCorners(vector<Mat> images, vector<vector<Point2f>> &allFoundCorners, bool showResult) {
    for (vector<Mat>::iterator iter = images.begin(); iter != images.end(); iter++) {
        vector<Point2f> pointBuffer;
        bool found = findChessboardCorners(*iter, boardSize, pointBuffer,
                                           CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE);
        if (found) {
            allFoundCorners.push_back(pointBuffer);
        }
        if (showResult) {
            drawChessboardCorners(*iter, boardSize, pointBuffer, found);
            imshow("Chessboard Corners", *iter);
            waitKey(0);
        }
    }
}

void Tracker::cameraCalibration(vector<Mat> images, Size boardSize, float squareEdgeLength, Mat &cameraMatrix,
                                Mat &distanceCoeffients) {
    vector<vector<Point2f>> chessboardCornerPoints;
    getChessboardCorners(images, chessboardCornerPoints, false);

    vector<vector<Point3f>> worldSpaceCornerPoints(1);

    createBoardPosition(boardSize, squareEdgeLength, worldSpaceCornerPoints[0]);
    worldSpaceCornerPoints.resize(chessboardCornerPoints.size(), worldSpaceCornerPoints[0]);
    cout << worldSpaceCornerPoints.size();

    vector<Mat> rVectors, tVectors;
    distanceCoeffients = Mat::zeros(8, 1, CV_64F);

    calibrateCamera(worldSpaceCornerPoints, chessboardCornerPoints, boardSize, cameraMatrix, distanceCoeffients,
                    rVectors, tVectors);

}

bool Tracker::saveCameraCalibration(string name, Mat cameraMatrix, Mat distanceCoeffients) {
    ofstream outstream(name);
    if (outstream) {
        uint16_t rows = cameraMatrix.rows;
        uint16_t cols = cameraMatrix.cols;

        outstream << rows << endl;
        outstream << cols << endl;

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                double value = cameraMatrix.at<double>(r, c);
                outstream << value << endl;
            }
        }

        rows = distanceCoeffients.rows;
        cols = distanceCoeffients.cols;

        outstream << rows << endl;
        outstream << cols << endl;

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                double value = distanceCoeffients.at<double>(r, c);
                outstream << value << endl;
            }
        }
        outstream.close();
        return true;
    }
    return false;
}

bool Tracker::loadCameraCalibration(string name, Mat &cameraMatrix, Mat &distanceCoeffients) {
    ifstream instream(name);
    if (instream) {
        uint16_t rows;
        uint16_t cols;

        rows = 3;
        cols = 3;

        cameraMatrix = Mat(Size(cols, rows), CV_64F);

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                double read = 0.0f;
                instream >> read;
                cameraMatrix.at<double>(r, c) = read;
                //cout << cameraMatrix.at<double>(r,c) << "\n";
            }
        }

        rows = 5;
        cols = 1;

        distanceCoeffients = Mat::zeros(rows, cols, CV_64F);

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                float read = 0.0f;
                instream >> read;
                distanceCoeffients.at<float>(r, c) = read;
                //cout << distanceCoeffients.at<double>(r,c) << "\n";
            }
        }
        instream.close();
        return true;
    }
    return false;
}

float SIGN(float x) {
    return (x >= 0.0f) ? +1.0f : -1.0f;
}

float NORM(float a, float b, float c, float d) {
    return sqrt(a * a + b * b + c * c + d * d);
}

// quaternion = [w, x, y, z]'
Mat mRot2Quat(const Mat &m) {
    float r11 = m.at<float>(0, 0);
    float r12 = m.at<float>(0, 1);
    float r13 = m.at<float>(0, 2);
    float r21 = m.at<float>(1, 0);
    float r22 = m.at<float>(1, 1);
    float r23 = m.at<float>(1, 2);
    float r31 = m.at<float>(2, 0);
    float r32 = m.at<float>(2, 1);
    float r33 = m.at<float>(2, 2);
    float q0 = (r11 + r22 + r33 + 1.0f) / 4.0f;
    float q1 = (r11 - r22 - r33 + 1.0f) / 4.0f;
    float q2 = (-r11 + r22 - r33 + 1.0f) / 4.0f;
    float q3 = (-r11 - r22 + r33 + 1.0f) / 4.0f;
    if (q0 < 0.0f) {
        q0 = 0.0f;
    }
    if (q1 < 0.0f) {
        q1 = 0.0f;
    }
    if (q2 < 0.0f) {
        q2 = 0.0f;
    }
    if (q3 < 0.0f) {
        q3 = 0.0f;
    }
    q0 = sqrt(q0);
    q1 = sqrt(q1);
    q2 = sqrt(q2);
    q3 = sqrt(q3);
    if (q0 >= q1 && q0 >= q2 && q0 >= q3) {
        q0 *= +1.0f;
        q1 *= SIGN(r32 - r23);
        q2 *= SIGN(r13 - r31);
        q3 *= SIGN(r21 - r12);
    } else if (q1 >= q0 && q1 >= q2 && q1 >= q3) {
        q0 *= SIGN(r32 - r23);
        q1 *= +1.0f;
        q2 *= SIGN(r21 + r12);
        q3 *= SIGN(r13 + r31);
    } else if (q2 >= q0 && q2 >= q1 && q2 >= q3) {
        q0 *= SIGN(r13 - r31);
        q1 *= SIGN(r21 + r12);
        q2 *= +1.0f;
        q3 *= SIGN(r32 + r23);
    } else if (q3 >= q0 && q3 >= q1 && q3 >= q2) {
        q0 *= SIGN(r21 - r12);
        q1 *= SIGN(r31 + r13);
        q2 *= SIGN(r32 + r23);
        q3 *= +1.0f;
    } else {
        printf("coding error\n");
    }
    float r = NORM(q0, q1, q2, q3);
    q0 /= r;
    q1 /= r;
    q2 /= r;
    q3 /= r;

    Mat res = (Mat_<float>(4, 1) << q0, q1, q2, q3);
    return res;
}

