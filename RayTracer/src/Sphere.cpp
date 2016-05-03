#include "Sphere.h"

#include <cmath>

Sphere::Sphere(float radius, Point position, IMaterial* outterMaterial, IMaterial* innerMaterial):radius_(radius), position_(position),
    outterMaterial_(outterMaterial), innerMaterial_(innerMaterial)
{
    auto ax = Point(1, 0, 0)*radius_;
    auto ay = Point(0, 1, 0)*radius_;
    auto az = Point(0, 0, 1)*radius_;
    this->boundingBox_ = Box((position_+ax).X, (position_-ax).X, (position_+ay).Y, (position_-ay).Y,
               (position_+az).Z, (position_-az).Z);
}

Box Sphere::GetBoundingBox() const
{
    return boundingBox_;
}

bool Sphere::GetCollision(Photon photon, CollisionData& collision)
{
    auto centDist = position_ - photon.Position();
    auto proj = centDist * photon.Direction().Normalized();
    auto heightLen2 = centDist*centDist - std::pow(proj, 2);
    if(proj < 0 || heightLen2 > radius_*radius_)
    {
        collision.IsCollide = false;
        return false;
    }
    Point intersecPoint(0, 0, 0);
    Point intersecNormal(0, 0, 0);
    auto heightPoint = photon.Direction().Normalized() *
            (centDist * photon.Direction().Normalized()) + photon.Position();
    auto sinus = std::sqrt(1 - heightLen2 / radius_ / radius_);
    collision.IsCollide = true;
    collision.Owner = this;
    if(centDist.Length() > radius_)
    {
        intersecPoint = heightPoint - photon.Direction().Normalized() * sinus * radius_;
        intersecNormal = (intersecPoint - position_).Normalized();
        collision.CollisionPoint = intersecPoint;
        collision.CollisionNormal = intersecNormal;
        collision.Material = this->outterMaterial_;
        if(outterMaterial_== nullptr)
            collision.PixelColor = Color(1, 1, 1);
        else
            collision.PixelColor = outterMaterial_->GetSelfColor();
    }
    else
    {
        intersecPoint = heightPoint + photon.Direction().Normalized() * sinus*radius_;
        intersecNormal = (intersecPoint - position_).Normalized() * -1;
        collision.CollisionPoint = intersecPoint;
        collision.CollisionNormal = intersecNormal;
        collision.Material = this->innerMaterial_;
        if(innerMaterial_== nullptr)
            collision.PixelColor = Color(1, 1, 1);
        else
            collision.PixelColor = innerMaterial_->GetSelfColor();
    }
    return true;
}

void Sphere::SetOutterMaterial(IMaterial* material)
{
    this->outterMaterial_ = material;
}

void Sphere::SetInnerMaterial(IMaterial* material)
{
    this->innerMaterial_ = material;
}
