#ifndef COLLISION_DATA_H
#define COLLISION_DATA_H

#include "Base.h"

struct CollisionData
{
    public:
        CollisionData(bool isCollide, Color color);
        bool IsCollide;
        Color PixelColor;
};

#endif
