#include "headers/edgeDetector.h"
#include "headers/camera.h"

int main(int argc, const char **argv) {
    std::string videoPath = "resources/movie.MP4";

    Camera *camera = new Camera(videoPath, new EdgeDetector());

    camera->setSlowMotion(10);
    camera->start();

    return 0;
}
