#include "Sphere.h"

#include <cmath>

Sphere::Sphere(float radius, Point position, IMaterial* material):radius_(radius), position_(position), material_(material)
{
}

Box Sphere::GetBoundingBox()
{
    auto ax = Point(1, 0, 0)*radius_;
    auto ay = Point(0, 1, 0)*radius_;
    auto az = Point(0, 0, 1)*radius_;
    return Box((position_+ax).X, (position_-ax).X, (position_+ay).Y, (position_-ay).Y,
               (position_+az).Z, (position_-az).Z);
}

CollisionData*Sphere::GetCollision(Photon photon)
{
    auto centDist = position_ - photon.Position();
    auto heightLen2 = centDist*centDist - std::pow(centDist * photon.Direction().Normalized(), 2);
    if(heightLen2 > radius_*radius_)
        return new CollisionData(false);
    Point intersecPoint(0, 0, 0);
    Point intersecNormal(0, 0, 0);
    auto heightPoint = photon.Direction().Normalized() *
            (centDist * photon.Direction().Normalized()) + photon.Position();
    auto sinus = std::sqrt(1 - heightLen2 / radius_ / radius_);
    if(centDist.Length() > radius_)
    {
        intersecPoint = heightPoint - photon.Direction() * sinus;
        intersecNormal = (intersecPoint - position_).Normalized();
    }
    else
    {
        intersecPoint = heightPoint + photon.Direction() * sinus ;
        intersecNormal = (intersecPoint - position_).Normalized() * -1;
    }
    return new CollisionData(true, material_->GetSelfColor(), intersecPoint, intersecNormal);
}
