#ifndef SPHERE_H
#define SPHERE_H

#include "ISceneObject.h"
#include "IMaterial.h"

class Sphere: public ISceneObject
{
    public:
        Sphere(float radius, Point position, IMaterial* outterMaterial = nullptr, IMaterial* innerMaterial = nullptr);
        Box GetBoundingBox() const;
        bool GetCollision(Photon photon, CollisionData& collision);
        void SetOutterMaterial(IMaterial* material);
        void SetInnerMaterial(IMaterial* material);
    private:
        float radius_;
        Point position_;
        IMaterial *outterMaterial_;
        IMaterial *innerMaterial_;
        Box boundingBox_;
};

#endif
