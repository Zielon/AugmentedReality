#include <LinearMath/btDefaultMotionState.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <GL/glut.h>
#include "../headers/grid.h"

using namespace std;

Grid::Grid(btScalar mass, btMotionState *motionState, btCollisionShape *collisionShape, const btVector3 &localInertia)
        : SceneObject(mass, motionState, collisionShape, localInertia) {
}

float Grid::gridSize = 10.f;
float Grid::gridThickness = 0.01f;

void Grid::draw() {
    glPushMatrix();
    getMotionState()->getWorldTransform(transform);
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
    trans.setOrigin(origin);

    motionState = new btDefaultMotionState(trans);

    btVector3 fallInertia(0, 0, 0);
    shape->calculateLocalInertia(0, fallInertia);

    auto grid = new Grid(0, motionState, shape, fallInertia);

    grid->setFriction(0.05f);
    grid->setCollisionFlags(grid->getCollisionFlags() | CF_KINEMATIC_OBJECT);
    grid->setActivationState(DISABLE_DEACTIVATION);
    grid->setUserIndex(-1);

    grid->transform = trans;
    grid->quaternion = qtn;
    grid->motionState = motionState;

    return grid;
}

Type Grid::getType() {
    return GRID;
}

void Grid::setRotation(float euler, float yaw, float pitch, float roll) {
    if (yaw > 0.0) this->gridYaw += euler;
    if (pitch > 0.0) this->gridPitch += euler;
    if (roll > 0.0) this->gridRoll += euler;
}

void Grid::update() {
    quaternion.setEuler(gridYaw, gridPitch, gridRoll);
    getMotionState()->getWorldTransform(transform);

    transform.setIdentity();
    transform.setRotation(quaternion);

    getMotionState()->setWorldTransform(transform);
    setWorldTransform(transform);
    motionState->setWorldTransform(transform);
}
