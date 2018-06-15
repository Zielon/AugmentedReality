#include "bounce/headers/application.h"

int main(int argc, char *argv[]) {

    Application app;

    app.initialize();
    app.start();

    return 0;
}