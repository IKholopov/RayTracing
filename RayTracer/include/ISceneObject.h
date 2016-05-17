#ifndef I_SCENE_OBJECT_H
#define I_SCENE_OBJECT_H

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
        virtual bool GetCollision(Photon photon, CollisionData& data) = 0;
        virtual void ApplyTransform(Transform A) = 0;
};

#endif
