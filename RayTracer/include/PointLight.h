#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "Base.h"
#include "CollisionData.h"
#include "IGeometryHierarchy.h"
#include "RenderConfig.h"

class CollisionData;
class IGeometryHierarchy;

class LightReference
{
    public:
        LightReference(float intensity, float distance);
        float GetIntensity() const;
        float GetDistance() const;
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
        Color EmitLight(CollisionData& collision, IGeometryHierarchy& hierarchy,
                        LightReference& reference, Color original, std::vector<PointLight*>& lights, RenderConfig& config);
        float AbsorbLight(CollisionData& collision, IGeometryHierarchy& hierarchy,
                        LightReference& reference, Color original, std::vector<PointLight*>& lights, RenderConfig& config);
        Point GetPosition() const;

    private:
        Point position_;
        float intensity_;
        Color light_;
};

#endif
