#ifndef I_GEOMETRY_HIERARHCY_H
#define I_GEOMETRY_HIERARHCY_H

#include "Base.h"
#include "Photon.h"
#include "PointLight.h"
#include "ISceneObject.h"

class IGeometryHierarchy
{
    public:
        virtual ~IGeometryHierarchy() {};
        virtual Color RenderPhoton(Photon photon, std::vector<PointLight*>& lights) = 0;
        virtual void Initialize(std::vector<ISceneObject*>& objects) = 0;
};

#endif
