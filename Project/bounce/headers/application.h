#ifndef PROJECT_APPLICATION_H
#define PROJECT_APPLICATION_H

#include <GLFW/glfw3.h>

class Application {
public:
    virtual void initialize();

    virtual void start();

    virtual void special(int key, int x, int y);

    virtual void specialUp(int key, int x, int y);

    virtual void idle();

    virtual void passiveMotion(int x, int y);

    virtual void motion(int x, int y);

    virtual void display();


private:
    GLFWwindow *window;
    double camera;

    static void reshape(GLFWwindow *window, int width, int height);

    static void mouse(GLFWwindow* window, double xpos, double ypos);

    static void keyboard(GLFWwindow* window, int key, int code, int action, int mods);

    static void keyboardUp(unsigned char key, int x, int y);
};

#endif //PROJECT_APPLICATION_H
