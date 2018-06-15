#ifndef PROJECT_APPLICATION_H
#define PROJECT_APPLICATION_H

class Application {
public:
    virtual void initialize();

    virtual void start();

    virtual void keyboard(unsigned char key, int x, int y);

    virtual void keyboardUp(unsigned char key, int x, int y);

    virtual void special(int key, int x, int y);

    virtual void specialUp(int key, int x, int y);

    virtual void reshape(int w, int h);

    virtual void idle();

    virtual void mouse(int button, int state, int x, int y);

    virtual void passiveMotion(int x, int y);

    virtual void motion(int x, int y);

    virtual void display();
};

#endif //PROJECT_APPLICATION_H
