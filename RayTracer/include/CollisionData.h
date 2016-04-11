#ifndef COLLISION_DATA_H
#define COLLISION_DATA_H

#include <vector>

#include "Base.h"
#include "Photon.h"

struct CollisionData
{
    public:
        CollisionData(bool isCollide);
        CollisionData(bool isCollide, Color color, Point collisionPoint, Point collisionNormal);
        bool IsCollide;
        Color PixelColor;
        Point CollisionPoint;
        Point CollisionNormal;
        std::vector<Photon> Photons;
};

#endif
