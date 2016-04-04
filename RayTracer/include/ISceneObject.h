#ifndef I_SCENE_OBJECT_H
#define I_SCENE_OBJECT_H

#include "Base.h"
#include "CollisionData.h"

class ISceneObject
{
    public:
        virtual ~ISceneObject();
        virtual Box GetBoundingBox() = 0;
        virtual CollisionData GetCollision() = 0;
};

#endif
