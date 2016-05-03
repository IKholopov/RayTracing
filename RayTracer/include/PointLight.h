#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "Base.h"
#include "CollisionData.h"
#include "IGeometryHierarchy.h"

class CollisionData;
class IGeometryHierarchy;

class PointLight
{
    public:
        PointLight(Point position, Color light, float intensity);
        Point GetPosition();
        Color GetLight();
        float GetIntensity();
        Color EmitLight(CollisionData& collision, IGeometryHierarchy& hierarchy);
    private:
        Point position_;
        float intensity_;
        Color light_;
};

#endif
