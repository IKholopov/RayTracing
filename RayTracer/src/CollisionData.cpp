#include "CollisionData.h"

CollisionData::CollisionData(bool isCollide):IsCollide(isCollide), Owner(nullptr), ReflectionDepth(0), RefractionDepth(0)
{}

CollisionData::CollisionData(bool isCollide, Color color, Point collisionPoint, Point collisionNormal,
                             IMaterial* material, Point photonDirection, float reflectionDepth, float refractionDepth):
    IsCollide(isCollide), PixelColor(color), CollisionPoint(collisionPoint),
    CollisionNormal(collisionNormal), Owner(nullptr), Material(material), PhotonDirection(photonDirection),
    ReflectionDepth(reflectionDepth), RefractionDepth(refractionDepth)
{}
