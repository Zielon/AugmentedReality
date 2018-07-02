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

    void update();

    Type getType() override;

    void setOrigin(btVector3 origin);

    btVector3 getOrigin();

    void setRotation(cv::Vec3d rotation);

    void setMatrix(double matrix[16]);

    static SceneObject *getDefault(btVector3 origin);

private:
    static float gridSize;
    static float gridThickness;
    btVector3 origin;
    btScalar matrix[16];
};

#endif //PROJECT_GRID_H
