#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "Base.h"

class PointLight
{
    public:
        PointLight(Point position, Color light);
        Point GetPosition();
        Color GetLight();
    private:
        Point position_;
        Color light_;
};

#endif
