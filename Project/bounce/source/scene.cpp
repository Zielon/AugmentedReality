#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletCollision/BroadphaseCollision/btAxisSweep3.h>
#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <GLFW/glfw3.h>
#include "../headers/scene_object.h"
#include "../headers/scene.h"
#include "../headers/ball.h"
#include "../headers/grid.h"

void Scene::addObject(SceneObject *element) {
    if (element != nullptr) {
        dynamicsWorld->addRigidBody(element);
        objects.push_back(element);
    }
}

void Scene::simulate() {
    float dtime = time;
    time = (float) glfwGetTime();
    dtime = time - dtime;

    dynamicsWorld->stepSimulation(dtime, 10);

    for (auto object : objects) object->draw();
}

Scene::Scene() {
    btDefaultCollisionConfiguration *collisionCfg = new btDefaultCollisionConfiguration();
    btAxisSweep3 *axisSweep = new btAxisSweep3(btVector3(-100, -100, -100), btVector3(100, 100, 100), 128);

    dynamicsWorld = new btDiscreteDynamicsWorld(new btCollisionDispatcher(collisionCfg), axisSweep,
                                                new btSequentialImpulseConstraintSolver, collisionCfg);

    dynamicsWorld->setGravity(btVector3(0, -9, 0));
}

void Scene::defaultSetting() {
    addObject(Ball::getDefault());
    addObject(Grid::getDefault());
}