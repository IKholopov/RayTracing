#ifndef I_SCENE_OBJECT_H
#define I_SCENE_OBJECT_H

#include<string>

#include "Base.h"
#include "CollisionData.h"
#include "Photon.h"
#include "IMaterial.h"
#include "Transform.h"

struct CollisionData;

class ISceneObject
{
    public:
        virtual ~ISceneObject() {};
        virtual const Box& GetBoundingBox() const = 0;
        virtual std::string GetType() const = 0;
        virtual IMaterial* GetPrimeMaterial() const = 0;
        virtual bool GetCollision(Photon photon, CollisionData& data) = 0;
        virtual void ApplyTransform(Transform A) = 0;
        virtual void ApplyInnerMaterial(IMaterial* material) = 0;
        virtual void ApplyOutterMaterial(IMaterial* material) = 0;
};

#endif
