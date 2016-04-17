#ifndef I_SCENE_OBJECT_H
#define I_SCENE_OBJECT_H

#include "Base.h"
#include "CollisionData.h"
#include "Photon.h"
#include "IMaterial.h"

struct CollisionData;

class ISceneObject
{
    public:
        virtual ~ISceneObject() {};
        virtual Box GetBoundingBox() = 0;
        virtual bool GetCollision(Photon photon, CollisionData& data) = 0;
};

#endif
