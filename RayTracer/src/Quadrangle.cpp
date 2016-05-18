#include "Quadrangle.h"

#include <assert.h>

Quadrangle::Quadrangle(Point p1, Point p2, Point p3, Point p4,
                             IMaterial* outterMaterial, IMaterial* innerMaterial): p1_(p1), p2_(p2), p3_(p3), p4_(p4),
                            outterMaterial_(outterMaterial), innerMaterial_(innerMaterial)
{
    assert(IsFloatZero((((p2_-p1_)^(p3_-p1_))^((p3_-p1_)^(p4_-p1_))).Length()));
    float XMax, YMax, ZMax, XMin, YMin, ZMin;
    XMax = p1_.X;
    if(p2_.X > XMax)
        XMax = p2_.X;
    if(p3_.X > XMax)
        XMax = p3_.X;
    if(p4_.X > XMax)
        XMax = p4_.X;
    YMax = p1_.Y;
    if(p2_.Y > YMax)
        YMax = p2_.Y;
    if(p3_.Y > YMax)
        YMax = p3_.Y;
    if(p4_.Y > YMax)
        YMax = p4_.Y;
    ZMax = p1_.Z;
    if(p2_.Z > ZMax)
        ZMax = p2_.Z;
    if(p3_.Z > ZMax)
        ZMax = p3_.Z;
    if(p4_.Z > ZMax)
        ZMax = p4_.Z;
    XMin = p1_.X;
    if(p2_.X < XMin)
        XMin = p2_.X;
    if(p3_.X < XMin)
        XMin = p3_.X;
    if(p4_.X < XMin)
        XMin = p4_.X;
    YMin = p1_.Y;
    if(p2_.Y < YMin)
        YMin = p2_.Y;
    if(p3_.Y < YMin)
        YMin = p3_.Y;
    if(p4_.Y < YMin)
        YMin = p4_.Y;
    ZMin = p1_.Z;
    if(p2_.Z < ZMin)
        ZMin = p2_.Z;
    if(p3_.Z < ZMin)
        ZMin = p3_.Z;
    if(p4_.Z < ZMin)
        ZMin = p4_.Z;
    boundingBox_ = Box(XMax, XMin, YMax, YMin, ZMax, ZMin);
}

const Box& Quadrangle::GetBoundingBox() const
{
    return boundingBox_;
}

bool Quadrangle::GetCollision(Photon photon, CollisionData& collision)
{
    auto norm = GetNormal();
    auto dist = p1_- photon.Position();
    auto height = norm * (dist * norm);
    if(IsFloatZero(height.Length()))
    {
        collision.IsCollide = false;
        return false;
    }
    auto cosin = height*photon.Direction() / (height.Length()) / photon.Direction().Length();
    if(cosin <= 0)
    {
        collision.IsCollide = false;
        return false;
    }
    auto intersecPoint = photon.Position() + photon.Direction().Normalized() * (height.Length() / cosin);
    auto v1 = p1_ - intersecPoint;
    auto v2 = p2_ - intersecPoint;
    auto v3 = p3_ - intersecPoint;
    auto v4 = p4_ - intersecPoint;
    auto n1 = v1^v2;
    auto n2 = v2^v3;
    auto n3 = v3^v4;
    auto n4 = v4^v1;
    if(n1*n2 > 0 && n2*n3 > 0 && n3*n4 > 0 && n4*n1 > 0)
    {
        collision.IsCollide = true;
        collision.Owner = this;
        collision.CollisionPoint = intersecPoint;
        if(height * norm < 0)
        {
            collision.Material = this->outterMaterial_;
            collision.PixelColor = outterMaterial_->GetSelfColor();
            collision.CollisionNormal = norm;
        }
        else
        {
            collision.Material = this->innerMaterial_;
            collision.PixelColor = innerMaterial_->GetSelfColor();
            collision.CollisionNormal = norm*(-1);\
        }
        return true;
    }
    collision.IsCollide = false;
    return false;
}

std::__cxx11::string Quadrangle::GetType() const
{
    return "quadrangle";
}

IMaterial*Quadrangle::GetPrimeMaterial() const
{
    return this->outterMaterial_;
}

void Quadrangle::SetOutterMaterial(IMaterial* material)
{
    this->outterMaterial_ = material;
}

void Quadrangle::SetInnerMaterial(IMaterial* material)
{
    this->innerMaterial_ = material;
}

void Quadrangle::ApplyTransform(Transform A)
{
    p1_ = A.Apply(p1_);
    p2_ = A.Apply(p2_);
    p3_ = A.Apply(p3_);
    p4_ = A.Apply(p4_);
}

void Quadrangle::ApplyOutterMaterial(IMaterial* material)
{
    this->outterMaterial_ = material;
}

void Quadrangle::ApplyInnerMaterial(IMaterial* material)
{
    this->innerMaterial_ = material;
}

Point Quadrangle::GetNormal()
{
    return ((p2_- p1_) ^ (p3_ - p1_)).Normalized();
}

Point Quadrangle::p4() const
{
    return p4_;
}

Point Quadrangle::p3() const
{
    return p3_;
}

Point Quadrangle::p2() const
{
    return p2_;
}

Point Quadrangle::p1() const
{
    return p1_;
}
