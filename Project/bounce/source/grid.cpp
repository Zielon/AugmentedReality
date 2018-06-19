#include <LinearMath/btDefaultMotionState.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include "../headers/grid.h"

Grid::Grid(btScalar mass, btMotionState *motionState, btCollisionShape *collisionShape, const btVector3 &localInertia)
        : SceneObject(mass, motionState, collisionShape, localInertia) {

}

void Grid::draw() {
    drawer.drawGrid();
}

void Grid::setPosition(double x, double y) {

}

SceneObject *Grid::getDefault(btVector3 origin, btVector3 boxShape, int size, bool gravity) {
    btQuaternion qtn;
    btCollisionShape *shape;
    btDefaultMotionState *motionState;
    btTransform trans;

    shape = new btBoxShape(boxShape);

    trans.setIdentity();
    qtn.setEuler(0.8, 0.7, 0.4);
    trans.setRotation(qtn);
    trans.setOrigin(origin);
    motionState = new btDefaultMotionState(trans);

    btScalar mass = gravity ? btScalar(1.0) : btScalar(0.0);
    btVector3 inertia = gravity ? btVector3(1, 1, 1) : btVector3(0, 0, 0);

    auto ball = new Grid(mass, motionState, shape, inertia);

    ball->transform = trans;
    ball->motionState = motionState;

    return ball;
}