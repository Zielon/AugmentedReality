#include <GL/gl.h>
#include <LinearMath/btDefaultMotionState.h>
#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <BulletCollision/BroadphaseCollision/btAxisSweep3.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include "../headers/ball_dynamics.h"

int OBJECTS_NUMBER = 2;

Ball::Ball() {

    btQuaternion qtn;
    btCollisionShape *shape;
    btDefaultMotionState *motionState;

    btDefaultCollisionConfiguration *collisionCfg = new btDefaultCollisionConfiguration();
    btAxisSweep3 *axisSweep = new btAxisSweep3(btVector3(-100, -100, -100), btVector3(100, 100, 100), 128);

    dynamicsWorld = new btDiscreteDynamicsWorld(new btCollisionDispatcher(collisionCfg), axisSweep,
                                                new btSequentialImpulseConstraintSolver, collisionCfg);

    dynamicsWorld->setGravity(btVector3(0, -10, 0));

    // STATIC
    shape = new btBoxShape(btVector3(20, 20, 20));

    trans.setIdentity();
    qtn.setEuler(0, 0.25, -0.05f);
    trans.setRotation(qtn);
    trans.setOrigin(btVector3(0, -20, 0));
    motionState = new btDefaultMotionState(trans);

    auto box = new btRigidBody(btScalar(0.0), motionState, shape, btVector3(0, 0, 0));
    dynamicsWorld->addRigidBody(box);
    objects.push_back(box);

    // DYNAMIC
    shape = new btBoxShape(btVector3(5, 5, 5));

    trans.setIdentity();
    qtn.setEuler(0.8, 0.7, 0.4);
    trans.setRotation(qtn);
    trans.setOrigin(btVector3(-10, 50, 0));
    motionState = new btDefaultMotionState(trans);

    box = new btRigidBody(btScalar(1.0), motionState, shape, btVector3(1, 1, 1));
    dynamicsWorld->addRigidBody(box);
    objects.push_back(box);
}

void Ball::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto size = objects.size();
    int models[size] = {40, 10};

    for (int i = 0; i < size; i++) {
        glColor3f(size, size / OBJECTS_NUMBER, 1.0);
        glPushMatrix();
        objects[i]->getMotionState()->getWorldTransform(trans);
        trans.getOpenGLMatrix(matrix);
        glMultMatrixf(matrix);
        glutSolidCube(models[i]);
        glPopMatrix();
    }
}

void Ball::simulate() {
    float dtime = time;
    time = (float) glfwGetTime();
    dtime = time - dtime;

    if (dynamicsWorld)
        dynamicsWorld->stepSimulation(dtime, 10);
}


