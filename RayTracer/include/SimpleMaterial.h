#ifndef SIMPLE_MATERIAL_H
#define SIMPLE_MATERIAL_H

#include "IMaterial.h"

class SimpleMaterial:public IMaterial
{
    public:
        SimpleMaterial(Color color);
        SimpleMaterial(Color color, float alpha, float n);
        Color RenderMaterial(IGeometryHierarchy& hierarchy, CollisionData* data, std::vector<PointLight*>& lights, RenderConfig& config, LightReference& reference);
        Color GetSelfColor();
        float GetAlpha() const;
        float GetN() const;

    private:
        Color color_;
        float alpha_;
        float n_;
};

#endif
