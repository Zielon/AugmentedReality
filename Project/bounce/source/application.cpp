#include <cmath>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>

#include "../headers/application.h"
#include "../headers/ball.h"
#include "../headers/scene.h"

using namespace std;

float cameraX = 0.0;
float cameraY = 0.0;
float cameraZ = 0.0;

void Application::keyboard(GLFWwindow *window, int key, int code, int action, int mods) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cameraY += 0.1;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cameraY -= 0.1;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cameraX += 0.1;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cameraX -= 0.1;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) cameraZ += 0.1;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) cameraZ -= 0.1;
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

void Application::display() {

    float ratio;
    int width, height;

    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float) height;

    glViewport(0, 0, width, height);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_COLOR_MATERIAL);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

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
    glTranslatef(0.0f, 0.0f, -20.0f);

    // move the object in a fancy way
    const float t = (float) glfwGetTime() * 2.0f;
    const float n = 0.5f;
    //glTranslatef(static_cast<GLfloat>(1.5f * sin(n * t)), 0.f, static_cast<GLfloat>(1.5f * cos(n * t)));

    glRotatef(20, 1.0, 0.0, 0.0);
    glTranslatef(cameraX, cameraY, cameraZ);
}

void Application::initialize() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

    glfwWindowHint(GLFW_SAMPLES, 5);

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    glClearDepth(1.0);
}

void Application::start() {

    if (!glfwInit()) return;

    window = glfwCreateWindow(1000, 800, "Bounce", nullptr, nullptr);

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

    glMatrixMode(GL_PROJECTION);
    gluPerspective(50.0, 1.0, 20.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    gluLookAt(0.0, 5.0, 90.0, 0.0, 8.0, 0.0, 0.0, 1.0, 0.0);

    Scene scene;

    // Set default objects [ 1 ball and 1 grid ]
    scene.defaultSetting();

    while (!glfwWindowShouldClose(window)) {

        scene.drawObjects();
        scene.simulateObjects();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}
