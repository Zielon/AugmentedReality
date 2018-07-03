#include <LinearMath/btDefaultMotionState.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>

#include "../headers/grid.h"

using namespace std;

Grid::Grid(btScalar mass, btMotionState *motionState, btCollisionShape *collisionShape, const btVector3 &localInertia)
        : SceneObject(mass, motionState, collisionShape, localInertia) {
}

float Grid::gridSize = 5.f;
float Grid::gridThickness = 0.05f;

void Grid::draw() {
    glPushMatrix();
    getMotionState()->getWorldTransform(transform);
    transform.getOpenGLMatrix(matrix);
    glMultMatrixf(matrix);
    drawer.drawGrid((int) gridSize, gridThickness, drawer.getGridPoints());
    glPopMatrix();
}

SceneObject *Grid::getDefault(btVector3 origin) {
    btQuaternion qtn;
    btCollisionShape *shape;
    btDefaultMotionState *motionState;
    btTransform trans;

    shape = new btBoxShape(btVector3(gridSize / 2, gridThickness / 2, gridSize / 2));

    trans.setIdentity();
    trans.setOrigin(origin);
    trans.setRotation(qtn);

    motionState = new btDefaultMotionState(trans);

    btVector3 fallInertia(0, 0, 0);
    shape->calculateLocalInertia(0, fallInertia);

    auto grid = new Grid(0, motionState, shape, fallInertia);

    grid->setFriction(0.5f);
    grid->setCollisionFlags(grid->getCollisionFlags() | CF_KINEMATIC_OBJECT);
    grid->setActivationState(DISABLE_DEACTIVATION);

    grid->transform = trans;
    grid->quaternion = qtn;
    grid->motionState = motionState;
    grid->origin = origin;

    return grid;
}

Type Grid::getType() {
    return GRID;
}

void Grid::update() {
    getMotionState()->getWorldTransform(transform);

    transform.setIdentity();

    transform.setRotation(quaternion);
    transform.setOrigin(origin);

    getMotionState()->setWorldTransform(transform);
    setWorldTransform(transform);
    motionState->setWorldTransform(transform);
}

void Grid::setRotation(cv::Vec3d rotation) {
    quaternion.setEuler(rotation[0], rotation[1], rotation[2]);
}

void Grid::setMatrix(double matrix[16]) {
    for (int i = 0; i < 16; i++)
        this->matrix[i] = (btScalar) matrix[i];
}

void Grid::setOrigin(btVector3 origin) {
    this->origin = origin;
}

btVector3 Grid::getOrigin() {
    return origin;
}
