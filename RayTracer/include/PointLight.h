#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "Base.h"

class PointLight
{
    public:
        PointLight(Point position, Color light, float intensity);
        Point GetPosition();
        Color GetLight();
        float GetIntensity();
    private:
        Point position_;
        float intensity_;
        Color light_;
};

#endif
