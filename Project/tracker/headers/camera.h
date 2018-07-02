#ifndef PROJECT_CAMERA_H
#define PROJECT_CAMERA_H

#include <opencv2/core/mat.hpp>
#include <opencv2/videoio.hpp>
#include <sstream>
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

class Camera {
public:
    Camera(int mode);

    void nextFrame(cv::Mat &frame);

    void createBoardPosition(Size boardSize, float squareEdgeLength, vector<Point3f> &corners);

    void getChessboardCorners(vector<Mat> images, vector<vector<Point2f>> &allFoundCorners, bool showResult = false);

    void cameraCalibration(vector<Mat> images, Size boardSize, float squareEdgeLength);

    bool saveCameraCalibration(string name);

    void loadCameraCalibration(string name);

    double *getProjection();

    Mat &getCameraMatrix();

    Mat &getDistortionCoefficient();

    float getSquareDimension();

    ~Camera();

private:
    VideoCapture *capture;
    Mat cameraMatrix;
    Mat distortionCoefficient;
    double projection[16];
    const float arucoSquareDimension = 0.049;
    const cv::Size boardSize = cv::Size(9, 6);

    void buildProjectionMatrix();
};

#endif //PROJECT_CAMERA_H
