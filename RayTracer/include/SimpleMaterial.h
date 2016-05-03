#ifndef SIMPLE_MATERIAL_H
#define SIMPLE_MATERIAL_H

#include "IMaterial.h"

class SimpleMaterial:public IMaterial
{
    public:
        SimpleMaterial(Color color);
        SimpleMaterial(Color color, float alpha);
        Color RenderMaterial(IGeometryHierarchy& hierarchy, CollisionData* data, std::vector<PointLight*>& lights);
        Color GetSelfColor();

    private:
        Color color_;
        float alpha_;
};

#endif
