#ifndef PROJECT_BALL_H
#define PROJECT_BALL_H

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

    static SceneObject *getDefault(btVector3 origin, float size, bool gravity);

private:
    float ballSize;
};

#endif //PROJECT_BALL_H
