#include <LinearMath/btDefaultMotionState.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include "../headers/grid.h"

Grid::Grid(btScalar mass, btMotionState *motionState, btCollisionShape *collisionShape, const btVector3 &localInertia)
        : SceneObject(mass, motionState, collisionShape, localInertia) {

}

void Grid::draw() {
    glPushMatrix();
    glRotatef(angleX, 1.0, 0.0, 0.0);
    glRotatef(angleY, 0.0, 1.0, 0.0);
    glRotatef(angleZ, 0.0, 0.0, 1.0);
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
    grid->motionState = motionState;

    return grid;
}

void Grid::setTransform(btTransform &worldTrans) {
    motionState->setWorldTransform(worldTrans);
}

Type Grid::getType() {
    return GRID;
}

void Grid::setRotation(float angle, float x, float y, float z) {
    if (x > 0.0) this->angleX += angle;
    if (y > 0.0) this->angleY += angle;
    if (z > 0.0) this->angleZ += angle;
}
