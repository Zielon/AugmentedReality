#include <opencv2/core/utility.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

Mat frame, grayFrame, binaryFrame;
int threshval = 20;

void toGray() {
	cvtColor(frame, grayFrame, CV_BGR2GRAY);
}

void toBinary() {
//	threshold(grayFrame, binaryFrame, threshval, 255, THRESH_BINARY);
	adaptiveThreshold(grayFrame, binaryFrame, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 11, 12);
//	adaptiveThreshold(grayFrame, binaryFrame, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 11, 12);
//	adaptiveThreshold(grayFrame, binaryFrame, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 11, 12);
}

int main(int argc, const char** argv)
{
	VideoCapture cap("movie.MP4");
	if (!cap.isOpened()) return -1;
	
	namedWindow("Threshold", 1);
	createTrackbar("Level", "Threshold", &threshval, 255);

	for (;;)
	{
		if (!cap.read(frame)) { 
			// Start from the beginning
			cap.set(CV_CAP_PROP_POS_FRAMES, 0);
			continue;
		};

		// Image transformations
		toGray();
		toBinary();

		imshow("Threshold", binaryFrame);

		char key = cvWaitKey(10);
		if (key == 27) break;
	}

	return 0;
}
