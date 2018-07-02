#include <bullet/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <bullet/BulletCollision/BroadphaseCollision/btAxisSweep3.h>
#include <bullet/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <algorithm>
#include <iterator>

#include "../headers/scene_object.h"
#include "../headers/scene.h"
#include "../headers/ball.h"

using namespace std;

Grid *Scene::grid = (Grid *) Grid::getDefault(btVector3(0, 0, 0));

void Scene::addObject(SceneObject *element) {
    if (element != nullptr) {
        if (objects.size() < 50) {
            dynamicsWorld->addRigidBody(element);
            objects.push_back(element);
        }
    }
}

void Scene::simulateObjects() {
    float dtime = time;
    time = (float) glfwGetTime();
    dtime = time - dtime;
    dynamicsWorld->stepSimulation(dtime);
}

Scene::Scene() {
    btDefaultCollisionConfiguration *collisionCfg = new btDefaultCollisionConfiguration();

    dynamicsWorld = new btDiscreteDynamicsWorld(
            new btCollisionDispatcher(collisionCfg),
            new btDbvtBroadphase(),
            new btSequentialImpulseConstraintSolver, collisionCfg
    );

    dynamicsWorld->setGravity(btVector3(0, -9.80665f, 0));
}

void Scene::defaultSetting() {
    addObject((SceneObject *) grid);
    addObject(Ball::getDefault(btVector3(1, 5, 0), 0.2));
    addObject(Ball::getDefault(btVector3(0, 5, 0), 0.2));
    addObject(Ball::getDefault(btVector3(-1, 5, 0), 0.2));
}

void Scene::drawObjects(double modelview[16]) {
    try {

        glMatrixMode(GL_PROJECTION);
        glLoadMatrixd(projection);

        glMatrixMode(GL_MODELVIEW);
        //glLoadMatrixd(modelview);

        glTranslatef(0, 0, -5);
        glScalef(0.25, 0.25, 0.25);

        for (auto object : objects) object->draw();

    } catch (const std::exception &e) { /* */ }
}

std::vector<SceneObject *> Scene::getObjects() {
    return objects;
}

void Scene::clear() {

    for (auto *body : objects) {
        dynamicsWorld->removeRigidBody(body);
        delete body->getMotionState();
        delete body;
    }

    objects.clear();

    delete dynamicsWorld;
}

void Scene::remove(bool all) {
    mutex.lock();

    vector<SceneObject *> visible;
    for (auto *body : objects) {
        auto y = body->getCenterOfMassPosition().getY();
        if (body->getType() == BALL) {
            if (!all && y > -7.0) {
                visible.push_back(body);
            } else {
                dynamicsWorld->removeRigidBody(body);
                delete body->getMotionState();
                delete body;
            };
        } else visible.push_back(body);
    }

    objects.clear();
    objects = visible;

    mutex.unlock();
}

void Scene::setProjectionMatrix(double projection[16]) {
    memcpy(this->projection, projection, 16 * sizeof(double));
}
