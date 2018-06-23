#include <opencv/cv.hpp>
#include "../headers/tracker.h"

using namespace cv;

Tracker::Tracker() {
    this->camera = new Camera();
}

void Tracker::findMatrix() {

}

void Tracker::defaultSetting() {

}

void Tracker::findMarker() {
    camera->nextFrame(mat);

    // DO STUFF WITH THE FRAME...
}

float *Tracker::getMatrix() {
    return this->matrix;
}
