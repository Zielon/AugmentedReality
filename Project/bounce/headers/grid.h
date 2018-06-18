#ifndef PROJECT_GRID_H
#define PROJECT_GRID_H

#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <LinearMath/btVector3.h>
#include <LinearMath/btMotionState.h>
#include "scene_object.h"

class Grid : SceneObject {
public:
    Grid(btScalar mass, btMotionState *motionState, btCollisionShape *collisionShape, const btVector3 &localInertia);

    void draw() override;

    void setPosition(double x, double y) override;

    static SceneObject *getDefault();
};

#endif //PROJECT_GRID_H
