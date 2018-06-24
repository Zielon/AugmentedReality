#include <opencv/cv.hpp>
#include "../headers/tracker.h"

using namespace cv;
using namespace std;

Tracker::Tracker() {
    this->camera = new Camera();
}

void Tracker::findMatrix() {

}

void Tracker::defaultSetting() {

}

void Tracker::findMarker() {
    camera->nextFrame(mat);
}

float *Tracker::getMatrix() {
    return this->matrix;
}

void Tracker::setMatrix(float *matrix) {
    mutex.lock();
    memcpy(this->matrix, matrix, sizeof(float) * 16);
    mutex.unlock();
}