#include "Sphere.h"

#include <cmath>

Sphere::Sphere(float radius, Point position, IMaterial* outterMaterial, IMaterial* innerMaterial):radius_(radius), position_(position),
    outterMaterial_(outterMaterial), innerMaterial_(innerMaterial)
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
    auto proj = centDist * photon.Direction().Normalized();
    auto heightLen2 = centDist*centDist - std::pow(proj, 2);
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
        if(outterMaterial_== nullptr)
            return new CollisionData(true, Color(1, 1, 1), intersecPoint, intersecNormal);
        return new CollisionData(true, outterMaterial_->GetSelfColor(), intersecPoint, intersecNormal);
    }
    else
    {
        intersecPoint = heightPoint + photon.Direction() * sinus ;
        intersecNormal = (intersecPoint - position_).Normalized() * -1;
        if(innerMaterial_== nullptr)
            return new CollisionData(true, Color(1, 1, 1), intersecPoint, intersecNormal);
        return new CollisionData(true, innerMaterial_->GetSelfColor(), intersecPoint, intersecNormal);
    }
}

void Sphere::SetOutterMaterial(IMaterial* material)
{
    this->outterMaterial_ = material;
}

void Sphere::SetInnerMaterial(IMaterial* material)
{
    this->innerMaterial_ = material;
}
