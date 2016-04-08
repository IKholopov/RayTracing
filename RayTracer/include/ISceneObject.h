#ifndef I_SCENE_OBJECT_H
#define I_SCENE_OBJECT_H

#include "Base.h"
#include "CollisionData.h"
#include "Photon.h"

class ISceneObject
{
    public:
        virtual ~ISceneObject() {};
        virtual Box GetBoundingBox() = 0;
        virtual CollisionData* GetCollision(Photon photon) = 0;
};

#endif
