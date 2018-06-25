#ifndef PROJECT_TRACKER_H
#define PROJECT_TRACKER_H

#include <opencv2/core/mat.hpp>
#include <GLFW/glfw3.h>
#include <thread>

#include "camera.h"

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/aruco/charuco.hpp>
#include <opencv2/calib3d.hpp>

#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

class Tracker {
public:
    Tracker();

    void findMatrix();

    void findMarker();

    void defaultSetting();

    float *getMatrix();

    void setMatrix(float *matrix);

    cv::Mat mat;
    
    void createBoardPosition(Size boardSize, float squareEdgeLength, vector<Point3f>& corners);
    
    void getChessboardCorners(vector<Mat> images, vector<vector<Point2f>>& allFoundCorners, bool showResult=false);
    
    void cameraCalibration(vector<Mat> images,Size boardSize, float squareEdgeLength, Mat& cameraMatrix, Mat& distanceCoeffients);
    
    bool saveCameraCalibration(string name, Mat cameraMatrix, Mat distanceCoeffients);
    
    bool loadCameraCalibration(string name, Mat& cameraMatrix, Mat& distanceCoeffients);
    
    void initVideoStream(cv::VideoCapture &cap);
    
    int detectMarker (Mat& cameraMatrix, Mat& distanceCoeffients);
    
    Mat getQuaternion(Mat& frame, Mat& cameraMatrix, Mat& distanceCoeffients);
    
private:

    Camera *camera;
    std::mutex mutex;
    float matrix[16];

    vector<Vec3d> rotationVectors, translationVectors;
    
    const cv::String webCam = "webCam";
    
    const float calibrationSquareDimension = 0.0247;    //meters
    
    const float arucoSquareDimension = 0.049;
    
    const Size boardSize = Size(9,6);
    
};

#endif //PROJECT_TRACKER_H
