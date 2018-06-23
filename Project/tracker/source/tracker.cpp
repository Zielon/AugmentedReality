#include <opencv/cv.hpp>
#include "../headers/tracker.h"

using namespace cv;
using namespace std;

Tracker::Tracker() {
    this->running = true;
}

void Tracker::findMatrix() {

}

void Tracker::defaultSetting() {

}

thread Tracker::findMarker() {
    return thread([this]{
        this->camera = new Camera();
        while(this->running){
            camera->nextFrame(mat);
        }
    });
}

float *Tracker::getMatrix() {
    return this->matrix;
}

void Tracker::setMatrix(float *matrix) {
    mutex.lock();
    memcpy(this->matrix, matrix, sizeof(float) * 16);
    mutex.unlock();
}

void Tracker::end(bool flag) {
    this->running = !flag;
}
