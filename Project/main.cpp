#include "bounce/headers/application.h"
#include "tracker/headers/tracker.h"

int main(int argc, char *argv[]) {

    Application app;

    app.start();
    
    Mat frame;
    Mat drawOnFrame;
    
    Mat cameraMatrix = Mat::eye(3,3,CV_64F);
    Mat distanceCoeffients;
    
    Tracker tracker;
    
    tracker.loadCameraCalibration("CameraMatrix", cameraMatrix, distanceCoeffients);
    tracker.detectMarker(cameraMatrix, distanceCoeffients);

    return 0;
}
