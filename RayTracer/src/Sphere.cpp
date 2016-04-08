#include "Sphere.h"

Sphere::Sphere(float radius, Point position):radius_(radius), position_(position)
{
}

Box Sphere::GetBoundingBox()
{
    auto ax = Point(1, 0, 0)*radius_;
    auto ay = Point(0, 1, 0)*radius_;
    auto az = Point(0, 0, 1)*radius_;
    return Box(position_+ax+ay+az, position_+ax+ay-az, position_+ax-ay+az, position_+ax-ay-az,
               position_-ax+ay+az, position_-ax+ay-az, position_-ax-ay+az, position_-ax-ay-az);
}

CollisionData*Sphere::GetCollision(Photon photon)
{

}
