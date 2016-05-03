#ifndef POLYGON_H
#define POLYGON_H

#include "ISceneObject.h"

class Polygon: public ISceneObject
{
    public:
        Polygon(Point p1, Point p2, Point p3,
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
        Box boundingBox_;
        IMaterial* outterMaterial_;
        IMaterial* innerMaterial_;
};

#endif
