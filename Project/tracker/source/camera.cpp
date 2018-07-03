#include <opencv2/videoio/videoio_c.h>
#include <opencv/cv.hpp>

#include "../headers/camera.h"
#include "../../bounce/headers/application.h"

using namespace cv;
using namespace std;

Camera::Camera(int mode) {

    switch (mode) {
        case 0:
            this->capture = new VideoCapture(0);
            break;
        case 1:
            this->capture = new VideoCapture("resources/movie");
            break;
        default:
            throw new Exception();
    }

    cameraMatrix = Mat::eye(3, 3, CV_64F);
    distortionCoefficient = Mat();

    loadCameraCalibration("resources/CameraMatrix");
    buildProjectionMatrix();
}

void Camera::nextFrame(cv::Mat &frame) {
    Mat src;
    if (!capture->isOpened()) return;

    if (!capture->read(src)) {
        capture->set(CV_CAP_PROP_POS_FRAMES, 0);
        capture->read(src);
    };

    flip(src, frame, 1);
    resize(frame, frame, Size(Application::WIDTH, Application::HEIGHT));
}

void Camera::createBoardPosition(Size boardSize, float squareEdgeLength, vector<Point3f> &corners) {
    for (int i = 0; i < boardSize.height; i++) {
        for (int j = 0; j < boardSize.width; j++) {
            corners.push_back(Point3f(j * squareEdgeLength, i * squareEdgeLength, 0.0f));
        }
    }
}

void Camera::getChessboardCorners(vector<Mat> images, vector<vector<Point2f>> &allFoundCorners, bool showResult) {
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

void Camera::cameraCalibration(vector<Mat> images, Size boardSize, float squareEdgeLength) {
    vector<vector<Point2f>> chessboardCornerPoints;
    getChessboardCorners(images, chessboardCornerPoints, false);

    vector<vector<Point3f>> worldSpaceCornerPoints(1);

    createBoardPosition(boardSize, squareEdgeLength, worldSpaceCornerPoints[0]);
    worldSpaceCornerPoints.resize(chessboardCornerPoints.size(), worldSpaceCornerPoints[0]);
    cout << worldSpaceCornerPoints.size();

    vector<Mat> rVectors, tVectors;
    distortionCoefficient = Mat::zeros(8, 1, CV_64F);

    calibrateCamera(worldSpaceCornerPoints, chessboardCornerPoints, boardSize, cameraMatrix, distortionCoefficient,
                    rVectors, tVectors);

}

bool Camera::saveCameraCalibration(string name) {
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

        rows = distortionCoefficient.rows;
        cols = distortionCoefficient.cols;

        outstream << rows << endl;
        outstream << cols << endl;

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                double value = distortionCoefficient.at<double>(r, c);
                outstream << value << endl;
            }
        }
        outstream.close();
        return true;
    }
    return false;
}

void Camera::loadCameraCalibration(string name) {
    ifstream stream(name);
    if (stream) {
        uint16_t rows;
        uint16_t cols;

        rows = 3;
        cols = 3;

        cameraMatrix = Mat(Size(cols, rows), CV_64F);

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                double read = 0.0f;
                stream >> read;
                cameraMatrix.at<double>(r, c) = read;
                //cout << cameraMatrix.at<double>(r,c) << "\n";
            }
        }

        rows = 5;
        cols = 1;

        distortionCoefficient = Mat::zeros(rows, cols, CV_64F);

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                float read = 0.0f;
                stream >> read;
                distortionCoefficient.at<float>(r, c) = read;
                //cout << distortionCoefficient.at<double>(r,c) << "\n";
            }
        }
        stream.close();
    } else {
        throw new Exception();
    }
}

void Camera::buildProjectionMatrix() {

    // Clipping
    float near = 0.01f;
    float far = 200.0f;

    float width = Application::WIDTH;
    float height = Application::HEIGHT;

    // Camera parameters
    double fx = cameraMatrix.at<double>(0, 0);
    double fy = cameraMatrix.at<double>(1, 1);
    double cx = cameraMatrix.at<double>(0, 2);
    double cy = cameraMatrix.at<double>(1, 2);

    projection[0] = 2.0f * fx / width;
    projection[1] = 0.0f;
    projection[2] = 0.0f;
    projection[3] = 0.0f;

    projection[4] = 0.0f;
    projection[5] = 2.0f * fy / height;
    projection[6] = 0.0f;
    projection[7] = 0.0f;

    projection[8] = (width - 2 * cx) / width;
    projection[9] = (height - 2 * cy) / height;
    projection[10] = (-far - near) / (far - near);
    projection[11] = -1.0f;

    projection[12] = 0.0f;
    projection[13] = 0.0f;
    projection[14] = -2.0f * far * near / (far - near);
    projection[15] = 0.0f;
}

Mat &Camera::getCameraMatrix() {
    return cameraMatrix;
}

Mat &Camera::getDistortionCoefficient() {
    return distortionCoefficient;
}

float Camera::getSquareDimension() {
    return arucoSquareDimension;
}

double *Camera::getProjection() {
    return projection;
}

Camera::~Camera() {
    this->capture->release();
    delete capture;
}