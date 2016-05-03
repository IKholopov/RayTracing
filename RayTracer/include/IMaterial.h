#ifndef I_MATERIAL_H
#define I_MATERIAL_H

#include <vector>

#include "Base.h"
#include "Photon.h"
#include "IGeometryHierarchy.h"
#include "PointLight.h"

class Photon;
class PointLight;

class IMaterial
{
    public:
        virtual ~IMaterial() {}
        virtual Color RenderMaterial(IGeometryHierarchy& hierarchy, CollisionData* data, std::vector<PointLight*>& lights) = 0;
        virtual Color GetSelfColor() = 0;
};

#endif
