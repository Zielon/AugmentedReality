#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <highgui.h>

using namespace cv;
using namespace std;

Mat frame, grayFrame, binaryFrame;
int _threshold = 60;
vector<vector<Point>> contours;
vector<Vec4i> hierarchy;

void toGray() {
    cvtColor(frame, grayFrame, CV_BGR2GRAY);
}

void toBinary() {
    threshold(grayFrame, binaryFrame, _threshold, 255, THRESH_BINARY);
    //	adaptiveThreshold(grayFrame, binaryFrame, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 11, 12);
    //	adaptiveThreshold(grayFrame, binaryFrame, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 11, 12);
    //  adaptiveThreshold(grayFrame, binaryFrame, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 11, 12);
}

void draw7points(Point a, Point b) {
    Point c = b - a;
    line(binaryFrame, a, b, Scalar(100, 100, 255), 2, 8, 0);
    for (int i = 1; i < 8; i++) {
        double indicator = (double) i / 7;
        Point point = indicator * c;
        circle(binaryFrame, a + point, 1, Scalar(150, 250, 250), 2, 8, 0);
    }
}

void extractShapes() {
    findContours(binaryFrame, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    for (int i = 0; i < contours.size(); i++) {
        vector<Point> approx;
        approxPolyDP(contours[i], approx, 3, true);
        if (approx.size() == 4) {
            Rect rect = boundingRect(Mat(approx));
            double area = rect.size().area();
            if (area > 1000) {
                //polylines(binaryFrame, approx, true, Scalar(100, 100, 255), 2, 8, 0);

                int p = 1;
                for (; p < approx.size(); p++)
                    draw7points(approx[p], approx[p - 1]);

                draw7points(approx[p - 1], approx[0]);

                for (int j = 0; j < approx.size(); j++) {
                    circle(binaryFrame, approx[j], 1, Scalar(100, 0, 255), 4, 8, 0);
                }
            }
        }
    }
}

int main(int argc, const char **argv) {
    VideoCapture cap("resources/movie.MP4");
    if (!cap.isOpened()) return -1;

    namedWindow("Threshold", 1);
    createTrackbar("Level", "Threshold", &_threshold, 255);

    for (;;) {
        if (!cap.read(frame)) {
            // Start from the beginning
            cap.set(CV_CAP_PROP_POS_FRAMES, 0);
            continue;
        };

        // Image transformations
        toGray();
        toBinary();
        extractShapes();

        imshow("Threshold", binaryFrame);

        std::this_thread::sleep_for(0.010s);

        char key = cvWaitKey(10);
        if (key == 27) break;
    }

    return 0;
}
