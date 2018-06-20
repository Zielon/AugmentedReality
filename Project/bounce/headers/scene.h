#ifndef PROJECT_SCENE_H
#define PROJECT_SCENE_H

#include <bullet/LinearMath/btScalar.h>
#include <bullet/LinearMath/btTransform.h>
#include <bullet/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <vector>

#include "scene_object.h"
#include "grid.h"

class Scene {
public:
    Scene();

    void addObject(SceneObject *element);

    std::vector<SceneObject *> getObjects();

    void simulateObjects();

    void drawObjects();

    void defaultSetting();

    void clear();

    void remove();

    static Grid *grid;

private:
    std::vector<SceneObject *> objects;
    btDiscreteDynamicsWorld *dynamicsWorld;
    btTransform trans;
    btScalar matrix[16];
    float time;
};

#endif //PROJECT_SCENE_H
