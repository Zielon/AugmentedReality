#ifndef PROJECT_DRAW_SNOWMAN_H
#define PROJECT_DRAW_SNOWMAN_H

#include "draw_primitives.h"

#include <GLFW/glfw3.h>
#include <cmath>

void drawSnowman() {
    glColor3f(1.0f, 1.0f, 1.0f);
    drawSphere(0.5, 40, 40);

    glTranslatef(0.0f, 0.60f, 0.0f);
    drawSphere(0.40, 40, 40);

    glTranslatef(0.0f, 0.50f, 0.0f);
    drawSphere(0.30, 40, 40);

    glPushMatrix();

    glColor3f(1.0f, 0.0f, 0.0f);

    glTranslatef(0.2f, 0.1f, 0.15f);
    drawSphere(0.05, 40, 40);

    glPopMatrix();
    glPushMatrix();

    glTranslatef(-0.2f, 0.1f, 0.15f);
    drawSphere(0.05, 40, 40);

    glPopMatrix();

    glTranslatef(0.0f, 0.0f, 0.1f);
    drawCone(0.05, 0.5, 10, 10);
}

#endif //PROJECT_DRAW_SNOWMAN_H
