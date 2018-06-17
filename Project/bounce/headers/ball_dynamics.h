#ifndef PROJECT_BALL_H
#define PROJECT_BALL_H

#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <vector>
#include "drawing.h"

class Ball {
public:
    Ball();
    void draw();
    void simulate();
private:
    Drawer drawer;
    std::vector<btRigidBody *> objects;
    btDiscreteDynamicsWorld *dynamicsWorld;
    btTransform trans;
    btScalar matrix[16];
    float time;
};

#endif //PROJECT_BALL_H
