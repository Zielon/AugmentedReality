#include <cmath>
#include <opencv/highgui.h>

#include "../headers/application.h"
#include "../headers/ball.h"

using namespace std;
using namespace cv;

float cameraX = 0.0;
float cameraY = 0.0;
float cameraZ = 0.0;

Scene *Application::scene = new Scene();
int Application::WINDOWS_WIDTH = 1280;
int Application::WINDOWS_HEIGHT = 720;

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

    if (mat.rows == 0 || mat.cols == 0) return;

    unsigned char pixels[WINDOWS_HEIGHT * WINDOWS_WIDTH * 3];

    memcpy(pixels, mat.data, sizeof(pixels));

    int width0, height0;
    glfwGetFramebufferSize(window, &width0, &height0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glOrtho(0.0, WINDOWS_WIDTH, 0.0, WINDOWS_HEIGHT, -1, 1);
    glRasterPos2i(0, WINDOWS_HEIGHT - 1);
    glDrawPixels(WINDOWS_WIDTH, WINDOWS_HEIGHT, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);

    glPopMatrix();

    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_MODELVIEW);
}

void Application::initialize() {
    glClearColor(0.0, 0.0, 0.0, 1.0);

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glPixelZoom(1.0, -1.0);

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glfwWindowHint(GLFW_SAMPLES, 5);
}

void Application::start() {

    if (!glfwInit()) return;

    window = glfwCreateWindow(WINDOWS_WIDTH, WINDOWS_HEIGHT, "Bounce", nullptr, nullptr);

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

        display(tracker->getFrame());

        tracker->findMarker();

        auto matrix = tracker->getMatrix();

        //scene->drawObjects(NULL);
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
