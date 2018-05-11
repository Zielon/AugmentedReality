#include "headers/edgeDetector.h"
#include "headers/camera.h"

int main(int argc, const char **argv) {
    std::string videoPath = "resources/movie";

    Camera *camera = new Camera(videoPath, new EdgeDetector());

    camera->setSlowMotion(100);
    camera->start();

    return 0;
}
