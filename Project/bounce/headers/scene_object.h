#ifndef PROJECT_OBJECT_H
#define PROJECT_OBJECT_H

#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

class SceneObject : public btRigidBody {
public:

    SceneObject(btScalar mass, btMotionState *motionState, btCollisionShape *collisionShape,
                const btVector3 &localInertia = btVector3(0, 0, 0))
            : btRigidBody(mass, motionState, collisionShape, localInertia) {
    }

    virtual void draw() = 0;

    virtual void setPosition(double x, double y) = 0;
};

#endif //PROJECT_OBJECT_H
