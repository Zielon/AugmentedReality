#ifndef PROJECT_DRAWING_H
#define PROJECT_DRAWING_H

#include <GL/gl.h>

class Drawer {
public:
    void drawSphere(double r, int lats, int longs);

    void drawCone(GLdouble base, GLdouble height, GLint slices, GLint stacks);

    void drawSnowman();

    void drawGrid();
};

#endif //PROJECT_DRAWING_H
