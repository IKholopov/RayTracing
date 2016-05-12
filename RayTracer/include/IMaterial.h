#ifndef I_MATERIAL_H
#define I_MATERIAL_H

#include <vector>

#include "Base.h"
#include "Photon.h"
#include "IGeometryHierarchy.h"
#include "PointLight.h"
#include "RenderConfig.h"

class Photon;
class PointLight;
class LightReference;

class IMaterial
{
    public:
        virtual ~IMaterial() {}
        virtual Color RenderMaterial(IGeometryHierarchy& hierarchy, CollisionData* data, std::vector<PointLight*>& lights, RenderConfig& config, LightReference& reference) = 0;
        virtual Color GetSelfColor() = 0;
};

#endif
