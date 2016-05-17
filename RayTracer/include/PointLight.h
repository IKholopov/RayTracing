#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "Base.h"
#include "CollisionData.h"
#include "IGeometryHierarchy.h"

class CollisionData;
class IGeometryHierarchy;

class LightReference
{
    public:
        LightReference(float intensity, float distance);
        float GetIntensity();
        float GetDistance();
    private:
        float intensity_;
        float distance_;
};

class PointLight
{
    public:
        PointLight(Point position, Color light, float intensity);
        Point GetPosition();
        Color GetLight();
        float GetIntensity();
        Color EmitLight(CollisionData& collision, IGeometryHierarchy& hierarchy, LightReference& reference, Color original);
    private:
        Point position_;
        float intensity_;
        Color light_;
};

#endif
