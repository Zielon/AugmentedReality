#include <LinearMath/btDefaultMotionState.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include "../headers/grid.h"

Grid::Grid(btScalar mass, btMotionState *motionState, btCollisionShape *collisionShape, const btVector3 &localInertia)
        : SceneObject(mass, motionState, collisionShape, localInertia) {

}

void Grid::draw() {
    glPushMatrix();
    drawer.drawGrid();
    glPopMatrix();
}

void Grid::setPosition(double x, double y) {

}

SceneObject *Grid::getDefault(btVector3 origin, btVector3 boxShape) {
    btQuaternion qtn;
    btCollisionShape *shape;
    btDefaultMotionState *motionState;
    btTransform trans;

    shape = new btBoxShape(boxShape);

    trans.setIdentity();
    qtn.setEuler(0.0, 0.0, 0.0);
    trans.setRotation(qtn);
    trans.setOrigin(origin);
    motionState = new btDefaultMotionState(trans);

    auto grid = new Grid(btScalar(0.0), motionState, shape, btVector3(0, 0, 0));

    grid->transform = trans;
    grid->quaternion = qtn;

    return grid;
}

void Grid::setAngle(btScalar x, btScalar y, btScalar z) {
    transform.setIdentity();
    quaternion.setEuler(x, y, z);
    transform.setRotation(quaternion);
}

Type Grid::getType() {
    return GRID;
}
