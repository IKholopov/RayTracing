#ifndef QUADRANGLE_H
#define QUADRANGLE_H

#include "ISceneObject.h"


class Quadrangle : public ISceneObject
{
    public:
        Quadrangle(Point p1, Point p2, Point p3, Point p4,
                IMaterial* outterMaterial = nullptr, IMaterial* innerMaterial = nullptr);
        const Box& GetBoundingBox() const;
        bool GetCollision(Photon photon, CollisionData& collision);
        std::string GetType() const;
        IMaterial* GetPrimeMaterial() const;
        void SetOutterMaterial(IMaterial* material);
        void SetInnerMaterial(IMaterial* material);
        void ApplyTransform(Transform A);
        void ApplyOutterMaterial(IMaterial* material);
        void ApplyInnerMaterial(IMaterial* material);
        Point p1() const;

        Point p2() const;

        Point p3() const;

        Point p4() const;

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
