#ifndef PROJECT_BALL_H
#define PROJECT_BALL_H

#ifdef __APPLE__
#include <gl.h>
#elif __linux__

#include <GL/gl.h>

#endif

#include <vector>
#include <bullet/LinearMath/btDefaultMotionState.h>

#include "drawing.h"
#include "scene_object.h"

class Ball : SceneObject {
public:
    Ball(btScalar mass, btMotionState *motionState, btCollisionShape *collisionShape, const btVector3 &localInertia);

    void draw() override;

    void setPosition(double x, double y) override;

    Type getType() override;

    static SceneObject *getDefault(btVector3 origin, float size);

private:
    float ballSize;
};

#endif //PROJECT_BALL_H
