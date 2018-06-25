#include "../headers/tracker.h"

using namespace cv;
using namespace std;

float SIGN(float x);
float NORM(float a, float b, float c, float d);
Mat mRot2Quat(const Mat &m);

Tracker::Tracker() {
    this->camera = new Camera();
    loadCameraCalibration("CameraMatrix", cameraMatrix, distanceCoeffients);
}

void Tracker::defaultSetting() {

}

void Tracker::findMarker() {
    camera->nextFrame(mat);
    
    vector<int> markerIds;
    vector<vector<Point2f>> markerCorners, rejectedCandidates;
    aruco::DetectorParameters parameters;
    Ptr<aruco::Dictionary> dictionary = aruco::getPredefinedDictionary(aruco::DICT_4X4_50);
    
    aruco::detectMarkers(mat, dictionary, markerCorners, markerIds);
    aruco::estimatePoseSingleMarkers(markerCorners, arucoSquareDimension, cameraMatrix, distanceCoeffients,
                                     rotationVectors, translationVectors);
    
}

float *Tracker::getMatrix() {
    
    Mat rotationMatrix = getRotationMatrix();
    Mat isotropyMatrix;
    
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            isotropyMatrix.at<double>(i,j) = rotationMatrix.at<double>(i,j);
        }
    }
    isotropyMatrix.at<double>(0,3) = translationVectors[0][0];
    isotropyMatrix.at<double>(1,3) = translationVectors[0][1];
    isotropyMatrix.at<double>(2,3) = translationVectors[0][2];
    
    isotropyMatrix.at<double>(3,0) = 0;
    isotropyMatrix.at<double>(3,1) = 0;
    isotropyMatrix.at<double>(3,2) = 0;
    isotropyMatrix.at<double>(3,3) = 1;

    //homography matrix = isotropy matrix multiplied by projection matrix
    //focalLength for projection Matrix???
    
    return this->matrix;
}

void Tracker::setMatrix(float *matrix) {                    //???
    mutex.lock();
    memcpy(this->matrix, matrix, sizeof(float) * 16);
    mutex.unlock();
}

Mat Tracker::getRotationMatrix(){
    Mat rotationMatrix;
    if (rotationVectors.size() != 0) {
        cout << rotationVectors[0] << endl;
        Rodrigues(rotationVectors[0], rotationMatrix);      //convert Rodriges angle to rotation matrix
    }
    
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
                                           CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE);
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
//                cout << cameraMatrix.at<double>(r,c) << "\n";
            }
        }

        rows = 5;
        cols = 1;

        distanceCoeffients = Mat::zeros(rows, cols, CV_64F);

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                double read = 0.0f;
                instream >> read;
                distanceCoeffients.at<double>(r, c) = read;
//                cout << distanceCoeffients.at<double>(r,c) << "\n";
            }
        }
        instream.close();
        return true;
    }
    return false;
}

int Tracker::detectMarkerTest() {
    Mat frame;
    vector<int> markerIds;
    vector<vector<Point2f>> markerCorners, rejectedCandidates;
    aruco::DetectorParameters parameters;

    Ptr<aruco::Dictionary> dictionary = aruco::getPredefinedDictionary(aruco::DICT_4X4_50);

    VideoCapture cap(0);
    if (!cap.isOpened())
        return -1;

    namedWindow(webCam, CV_WINDOW_AUTOSIZE);

    while (1) {
        if (!cap.read(frame))
            break;

        aruco::detectMarkers(frame, dictionary, markerCorners, markerIds);
        aruco::estimatePoseSingleMarkers(markerCorners, arucoSquareDimension, cameraMatrix, distanceCoeffients,
                                         rotationVectors, translationVectors);


        for (int i = 0; i < markerIds.size(); i++) {
            aruco::drawAxis(frame, cameraMatrix, distanceCoeffients, rotationVectors, translationVectors, 0.1f);
        }

        imshow(webCam, frame);
        if (waitKey(30) >= 0) break;
    }
    return 1;
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
