#ifndef SPHERE_H
#define SPHERE_H

#include "ISceneObject.h"
#include "IMaterial.h"

class Sphere: public ISceneObject
{
    public:
        Sphere(float radius, Point position, IMaterial* material);
        Box GetBoundingBox();
        CollisionData* GetCollision(Photon photon);
    private:
        float radius_;
        Point position_;
        IMaterial *material_;
};

#endif
