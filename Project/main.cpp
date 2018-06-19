#include <GL/glut.h>
#include "bounce/headers/application.h"

int main(int argc, char *argv[]) {

    // temporary only for drawing
    glutInit(&argc, argv);

    Application app;

    app.start();

    return 0;
}