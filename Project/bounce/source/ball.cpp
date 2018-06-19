#include <GL/gl.h>
#include <LinearMath/btDefaultMotionState.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include "../headers/ball.h"

Ball::Ball(btScalar mass,
           btMotionState *motionState,
           btCollisionShape *collisionShape,
           const btVector3 &localInertia)
        : SceneObject(mass, motionState, collisionShape, localInertia) {
}

void Ball::draw() {
    glPushMatrix();
    ((btRigidBody *) this)->getMotionState()->getWorldTransform(transform);
    transform.getOpenGLMatrix(matrix);
    glMultMatrixf(matrix);
    drawer.drawSphere(ballSize, 50, 50);
    glPopMatrix();
}

void Ball::setPosition(double x, double y) {}

SceneObject *Ball::getDefault(btVector3 origin, float size, bool gravity) {
    btQuaternion qtn;
    btCollisionShape *shape;
    btDefaultMotionState *motionState;
    btTransform trans;

    shape = new btSphereShape(size);

    trans.setIdentity();
    qtn.setEuler(0.0, 0.0, 0.0);
    trans.setRotation(qtn);
    trans.setOrigin(origin);

    motionState = new btDefaultMotionState(trans);

    btScalar mass = gravity ? btScalar(1) : btScalar(0.0);
    btVector3 inertia = gravity ? btVector3(1, 1, 1) : btVector3(0, 0, 0);

    auto ball = new Ball(mass, motionState, shape, inertia);

    ball->setFriction(1);
    ball->quaternion = qtn;
    ball->ballSize = size;
    ball->motionState = motionState;

    return ball;
}

Type Ball::getType() {
    return BALL;
}
