#include "../headers/grid.h"

Grid::Grid(btScalar mass, btMotionState *motionState, btCollisionShape *collisionShape, const btVector3 &localInertia)
        : SceneObject(mass, motionState, collisionShape, localInertia) {

}

void Grid::draw() {

}

void Grid::setPosition(double x, double y) {

}

SceneObject *Grid::getDefault() {
    return nullptr;
}
