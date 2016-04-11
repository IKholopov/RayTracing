#include "CollisionData.h"

CollisionData::CollisionData(bool isCollide):IsCollide(isCollide)
{}

CollisionData::CollisionData(bool isCollide, Color color, Point collisionPoint, Point collisionNormal):
    IsCollide(isCollide), PixelColor(color), CollisionPoint(collisionPoint), CollisionNormal(collisionNormal)
{}
