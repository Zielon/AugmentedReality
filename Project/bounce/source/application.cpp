#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include "../headers/application.h"
#include "../headers/drawing.h"

void Application::keyboard(unsigned char key, int x, int y) {

}

void Application::keyboardUp(unsigned char key, int x, int y) {

}

void Application::special(int key, int x, int y) {

}

void Application::specialUp(int key, int x, int y) {

}

void Application::reshape(GLFWwindow *window, int width, int height){
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

void Application::mouse(int button, int state, int x, int y) {

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

    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_COLOR_MATERIAL);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

//  glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
//  gluPerspective( 30, ((double)width/(double)height), 0.01, 100 );

    int fov = 30;
    float near = 0.01f, far = 100.f;
    float top = static_cast<float>(tan(fov * M_PI / 360.0f) * near);
    float bottom = -top;
    float left = ratio * bottom;
    float right = ratio * top;
    glFrustum(left, right, bottom, top, near, far);
    // move to origin
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // move the object backwards
    glTranslatef(0.0f, 0.0f, -10.0f);

    // move the object in a fancy way
    const float t = (float) glfwGetTime() * 2.0f;
    const float n = 0.5f;
    glTranslatef(1.5f * sin(n * t), 0.f, 1.5f * cos(n * t));

    // rotate the object
    glRotatef((float) glfwGetTime() * 50.f, 1.f, 0.f, 1.f);

    Drawer drawer;

    drawer.drawSnowman();

    GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void Application::initialize() {

    // enable and set colors
    glEnable(GL_COLOR_MATERIAL);
    glClearColor(0.0, 0.0, 0.0, 1.0);

    // enable and set depth parameters
    glEnable(GL_DEPTH_TEST);

    glClearDepth(1.0);
}

void Application::start() {

    if (!glfwInit()) return;

    window = glfwCreateWindow(640, 480, "Bounce", NULL, NULL);

    if (!window) {
        glfwTerminate();
        return;
    }

    // Set callback functions for GLFW
    glfwSetFramebufferSizeCallback(window, Application::reshape);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // initialize the GL library
    initialize();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        display();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
}
