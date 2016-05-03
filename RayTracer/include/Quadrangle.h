#ifndef QUADRANGLE_H
#define QUADRANGLE_H

#include "ISceneObject.h"


class Quadrangle : public ISceneObject
{
    public:
        Quadrangle(Point p1, Point p2, Point p3, Point p4,
                IMaterial* outterMaterial = nullptr, IMaterial* innerMaterial = nullptr);
        Box GetBoundingBox() const;
        bool GetCollision(Photon photon, CollisionData& collision);
        void SetOutterMaterial(IMaterial* material);
        void SetInnerMaterial(IMaterial* material);
    private:
        Point GetNormal();
        Point p1_;
        Point p2_;
        Point p3_;
        Point p4_;
        Box boundingBox_;
        IMaterial* outterMaterial_;
        IMaterial* innerMaterial_;
};

#endif
