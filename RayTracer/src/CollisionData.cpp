#include "CollisionData.h"

CollisionData::CollisionData(bool isCollide):IsCollide(isCollide), Owner(nullptr)
{}

CollisionData::CollisionData(bool isCollide, Color color, Point collisionPoint, Point collisionNormal,
                             IMaterial* material, Point photonDirection, float depth):
    IsCollide(isCollide), PixelColor(color), CollisionPoint(collisionPoint),
    CollisionNormal(collisionNormal), Owner(nullptr), Material(material), PhotonDirection(photonDirection), Depth(depth)
{}
