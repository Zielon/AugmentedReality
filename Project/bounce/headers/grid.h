#ifndef PROJECT_GRID_H
#define PROJECT_GRID_H

#include <bullet/BulletCollision/CollisionShapes/btCollisionShape.h>
#include <bullet/LinearMath/btVector3.h>
#include <bullet/LinearMath/btMotionState.h>
#include "scene_object.h"
#include "drawing.h"

class Grid : SceneObject {
public:
    Grid(btScalar mass, btMotionState *motionState, btCollisionShape *collisionShape, const btVector3 &localInertia);

    void draw() override;

    void setPosition(double x, double y) override;

    Type getType() override;

    void setRotation(float angle, float x, float y, float z);

    static SceneObject *getDefault(btVector3 origin);

private:
    static float gridSize;
    float angleX = .0;
    float angleY = .0;
    float angleZ = .0;
};

#endif //PROJECT_GRID_H
