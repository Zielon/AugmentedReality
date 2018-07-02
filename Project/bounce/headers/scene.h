#ifndef PROJECT_SCENE_H
#define PROJECT_SCENE_H

#include <bullet/LinearMath/btScalar.h>
#include <bullet/LinearMath/btTransform.h>
#include <bullet/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <vector>
#include <mutex>

#include "scene_object.h"
#include "grid.h"
#include "../../tracker/headers/camera.h"

class Scene {
public:
    Scene();

    void addObject(SceneObject *element);

    std::vector<SceneObject *> getObjects();

    void simulateObjects();

    void setProjectionMatrix(double projection[16]);

    void drawObjects(double *modelview);

    void defaultSetting();

    void clear();

    void remove(bool all);

    static Grid *grid;

private:
    std::vector<SceneObject *> objects;
    btDiscreteDynamicsWorld *dynamicsWorld;
    btTransform trans;
    double projection[16];
    float time;
    std::mutex mutex;
};

#endif //PROJECT_SCENE_H
