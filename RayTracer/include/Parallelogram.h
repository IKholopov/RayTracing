#ifndef PARALLELOGRAM_H
#define PARALLELOGRAM_H

#include "ISceneObject.h"


class Parallelogram : public ISceneObject
{
    public:
        Parallelogram(Point pivot, Point left, Point right,
                IMaterial* outterMaterial = nullptr, IMaterial* innerMaterial = nullptr);
        Box GetBoundingBox();
        CollisionData*GetCollision(Photon photon);
        void SetOutterMaterial(IMaterial* material);
        void SetInnerMaterial(IMaterial* material);
    private:
        Point GetNormal();
        Point pivot_;
        Point left_;
        Point right_;
        Box boundingBox_;
        IMaterial* outterMaterial_;
        IMaterial* innerMaterial_;
};

#endif