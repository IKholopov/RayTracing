#ifndef I_GEOMETRY_HIERARHCY_H
#define I_GEOMETRY_HIERARHCY_H

#include "Base.h"
#include "Photon.h"
#include "PointLight.h"
#include "ISceneObject.h"
#include "CollisionData.h"

class Photon;
class ISceneObject;
class CollisionData;

class IGeometryHierarchy
{
    public:
        virtual ~IGeometryHierarchy() {};
        virtual CollisionData* RenderPhoton(Photon photon) = 0;
        virtual CollisionData* CheckCollide(const Photon& photon) = 0;
        virtual void Initialize(std::vector<ISceneObject*>& objects) = 0;
};

#endif
