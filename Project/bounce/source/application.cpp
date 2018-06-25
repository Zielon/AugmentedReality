#include <cmath>
#include <opencv/highgui.h>
#include <opencv2/imgproc.hpp>

#include "../headers/application.h"
#include "../headers/ball.h"

using namespace std;
using namespace cv;

float cameraX = 0.0;
float cameraY = 0.0;
float cameraZ = 0.0;

Scene *Application::scene = new Scene();
int Application::WINDOWS_WIDTH = 800;
int Application::WINDOWS_HEIGHT = 800;

void Application::keyboard(GLFWwindow *window, int key, int code, int action, int mods) {

    // ========== GRID ROTATION ==========

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) Scene::grid->setRotation(-0.025f, 0.f, 0.f, 1.f);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) Scene::grid->setRotation(0.025, 0.f, 0.f, 1.f);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) Scene::grid->setRotation(0.025, 0.f, 1.f, 0.f);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) Scene::grid->setRotation(-0.025f, 0.f, 1.f, 0.f);

    // ==========

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cameraY += 0.1;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cameraY -= 0.1;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cameraX -= 0.1;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cameraX += 0.1;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) cameraZ += 0.1;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) cameraZ -= 0.1;
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) scene->remove(true);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        scene->addObject(Ball::getDefault(btVector3(0, 5, 0), 0.3));
    }
}

void Application::keyboardUp(unsigned char key, int x, int y) {

}

void Application::special(int key, int x, int y) {

}

void Application::specialUp(int key, int x, int y) {

}

void Application::reshape(GLFWwindow *window, int width, int height) {
    // set a whole-window viewport
    glViewport(0, 0, (GLsizei) width, (GLsizei) height);

    // create a perspective projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective( 30, ((GLfloat)width/(GLfloat)height), 0.01, 100 );
    float ratio = (GLfloat) width / (GLfloat) height;
    int fov = 30;
    float near = 0.01f, far = 100.f;
    float top = static_cast<float>(tan(fov * M_PI / 360.0f) * near);
    float bottom = -top;
    float left = ratio * bottom;
    float right = ratio * top;
    glFrustum(left, right, bottom, top, near, far);
}

void Application::idle() {

}

void Application::mouse(GLFWwindow *window, double xpos, double ypos) {

}

void Application::passiveMotion(int x, int y) {

}

void Application::motion(int x, int y) {

}

void Application::display(Mat &mat) {

    unsigned char pixels[WINDOWS_HEIGHT * WINDOWS_WIDTH * 3];
    Size size(WINDOWS_HEIGHT, WINDOWS_WIDTH);
    Mat windowPixels;
    resize(mat, windowPixels, size);
    memcpy(pixels, windowPixels.data, sizeof(pixels));

    float ratio;
    int width, height;

    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float) height;

    glViewport(0, 0, width, height);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);

    glPushMatrix();
    glDrawPixels(WINDOWS_HEIGHT, WINDOWS_WIDTH, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
    glPopMatrix();

    glEnable(GL_DEPTH_TEST);

    int fov = 30;
    float near = 0.01f, far = 100.f;
    auto top = static_cast<float>(tan(fov * M_PI / 360.0f) * near);
    float bottom = -top;
    float left = ratio * bottom;
    float right = ratio * top;

    glFrustum(left, right, bottom, top, near, far);
    // move to origin
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // move the object backwards
    glTranslatef(0.0f, 0.0f, -25.0f);
    glRotatef(10, 1.0, 0.0, 0.0);
    glTranslatef(cameraX, cameraY, cameraZ);
}

void Application::initialize() {
    glClearColor(0.0, 0.0, 0.0, 1.0);

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glfwWindowHint(GLFW_SAMPLES, 5);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
}

void Application::start() {

    if (!glfwInit()) return;

    window = glfwCreateWindow(WINDOWS_HEIGHT, WINDOWS_WIDTH, "Bounce", nullptr, nullptr);

    if (!window) {
        glfwTerminate();
        return;
    }

    // The register glfw callbacks
    glfwSetFramebufferSizeCallback(window, Application::reshape);
    glfwSetKeyCallback(window, Application::keyboard);
    glfwSetCursorPosCallback(window, Application::mouse);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    initialize();

    auto *tracker = new Tracker();

    scene->defaultSetting();
    tracker->defaultSetting();

    while (!glfwWindowShouldClose(window)) {

        tracker->detectMarker();

        auto matrix = tracker->getMatrix();

        display(tracker->getFrame());

        scene->drawObjects(matrix);
        scene->simulateObjects();
        scene->remove(false);

        Scene::grid->update();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    scene->clear();
    delete scene;

    glfwTerminate();
}
