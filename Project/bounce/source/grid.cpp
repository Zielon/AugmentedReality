#include <LinearMath/btDefaultMotionState.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <GL/glut.h>
#include "../headers/grid.h"

Grid::Grid(btScalar mass, btMotionState *motionState, btCollisionShape *collisionShape, const btVector3 &localInertia)
        : SceneObject(mass, motionState, collisionShape, localInertia) {
}

float Grid::gridSize = 7.f;
float Grid::gridThickness = 0.1f;

void Grid::draw() {
    glPushMatrix();
    quaternion.setEuler(angleX, angleY, angleZ);
    transform.setIdentity();
    transform.setRotation(quaternion);
    motionState->setWorldTransform(transform);
    setMotionState(motionState);

    transform.getOpenGLMatrix(matrix);
    glMultMatrixf(matrix);
    drawer.drawGrid((int) gridSize, gridThickness);
    glPopMatrix();
}

void Grid::setPosition(double x, double y) {

}

SceneObject *Grid::getDefault(btVector3 origin) {
    btQuaternion qtn;
    btCollisionShape *shape;
    btDefaultMotionState *motionState;
    btTransform trans;

    shape = new btBoxShape(btVector3(gridSize / 2, gridThickness / 2, gridSize / 2));

    trans.setIdentity();
    qtn.setEuler(0, 0, 0);
    trans.setRotation(qtn);
    trans.setOrigin(origin);

    motionState = new btDefaultMotionState(trans);

    auto grid = new Grid(btScalar(0.0), motionState, shape, btVector3(0, 0, 0));

    grid->transform = trans;
    grid->quaternion = qtn;
    grid->motionState = motionState;

    return grid;
}

Type Grid::getType() {
    return GRID;
}

void Grid::setRotation(float angle, float x, float y, float z) {
    if (x > 0.0) this->angleX += angle;
    if (y > 0.0) this->angleY += angle;
    if (z > 0.0) this->angleZ += angle;
}
