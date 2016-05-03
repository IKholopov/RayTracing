#ifndef COLLISION_DATA_H
#define COLLISION_DATA_H

#include <vector>

#include "Base.h"
#include "Photon.h"
#include "ISceneObject.h"
#include "IMaterial.h"

class ISceneObject;
class IMaterial;
class Photon;

struct CollisionData
{
    public:
        CollisionData(bool isCollide);
        CollisionData(bool isCollide, Color color, Point collisionPoint, Point collisionNormal,
                      IMaterial* material, Point photonDirection, float depth = 0);
        bool IsCollide;
        Color PixelColor;
        Point CollisionPoint;
        Point CollisionNormal;
        ISceneObject* Owner;
        std::vector<Photon> Photons;
        IMaterial* Material;
        Point PhotonDirection;
        float Depth;
};

#endif
