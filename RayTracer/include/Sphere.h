#ifndef SPHERE_H
#define SPHERE_H

#include "ISceneObject.h"

class Sphere: public ISceneObject
{
    public:
        Sphere(float radius, Point position);
        Box GetBoundingBox();
        CollisionData* GetCollision(Photon photon);
    private:
        float radius_;
        Point position_;
};

#endif
