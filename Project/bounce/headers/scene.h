#ifndef PROJECT_SCENE_H
#define PROJECT_SCENE_H

#include <LinearMath/btScalar.h>
#include <LinearMath/btTransform.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <vector>
#include "scene_object.h"

class Scene {
public:
    Scene();

    void addObject(SceneObject *element);

    void simulate();

    void defaultSetting();

private:
    std::vector<SceneObject *> objects;
    btDiscreteDynamicsWorld *dynamicsWorld;
    btTransform trans;
    btScalar matrix[16];
    float time;
};

#endif //PROJECT_SCENE_H
