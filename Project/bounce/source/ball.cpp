#include <GL/gl.h>
#include <GL/glut.h>
#include <LinearMath/btDefaultMotionState.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include "../headers/ball.h"

Ball::Ball(btScalar mass,
           btMotionState *motionState,
           btCollisionShape *collisionShape,
           const btVector3 &localInertia)
        : SceneObject(mass, motionState, collisionShape, localInertia) {
    this->motionState = motionState;
}

void Ball::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(0, 1.0f, 1.0);
    glPushMatrix();
    ((btRigidBody *) this)->getMotionState()->getWorldTransform(transform);
    transform.getOpenGLMatrix(matrix);
    glMultMatrixf(matrix);
    glutSolidCube(10);
    glPopMatrix();
}

void Ball::setPosition(double x, double y) {}

SceneObject *Ball::getDefault() {
    btQuaternion qtn;
    btCollisionShape *shape;
    btDefaultMotionState *motionState;
    btTransform trans;

    // DYNAMIC DEFAULT OBJECT
    shape = new btBoxShape(btVector3(5, 5, 5));

    trans.setIdentity();
    qtn.setEuler(0.8, 0.7, 0.4);
    trans.setRotation(qtn);
    trans.setOrigin(btVector3(-10, 50, 0));
    motionState = new btDefaultMotionState(trans);

    auto ball = new Ball(btScalar(1.0), motionState, shape, btVector3(1, 1, 1));

    ball->transform = trans;
    ball->motionState = motionState;

    return ball;
}

void Ball::setTransform(btTransform transform) {
    this->transform = transform;
}
