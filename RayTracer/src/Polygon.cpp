#include "Polygon.h"

#include <utility>

Polygon::Polygon(Point p1, Point p2, Point p3, IMaterial* outterMaterial, IMaterial* innerMaterial):p1_(p1), p2_(p2), p3_(p3),
    outterMaterial_(outterMaterial), innerMaterial_(innerMaterial)
{
    float XMax, YMax, ZMax, XMin, YMin, ZMin;
    XMax = p1_.X;
    if(p2_.X > XMax)
        XMax = p2_.X;
    if(p3_.X > XMax)
        XMax = p3_.X;
    YMax = p1_.Y;
    if(p2_.Y > YMax)
        YMax = p2_.Y;
    if(p3_.Y > YMax)
        YMax = p3_.Y;
    ZMax = p1_.Z;
    if(p2_.Z > ZMax)
        ZMax = p2_.Z;
    if(p3_.Z > ZMax)
        ZMax = p3_.Z;
    XMin = p1_.X;
    if(p2_.X < XMin)
        XMin = p2_.X;
    if(p3_.X < XMin)
        XMin = p3_.X;
    YMin = p1_.Y;
    if(p2_.Y < YMin)
        YMin = p2_.Y;
    if(p3_.Y < YMin)
        YMin = p3_.Y;
    ZMin = p1_.Z;
    if(p2_.Z < ZMin)
        ZMin = p2_.Z;
    if(p3_.Z < ZMin)
        ZMin = p3_.Z;
    boundingBox_ = Box(XMax, XMin, YMax, YMin, ZMax, ZMin);
}

Box Polygon::GetBoundingBox()
{
    return boundingBox_;
}

CollisionData* Polygon::GetCollision(Photon photon)
{
    auto norm = GetNormal();
    auto dist = p1_ - photon.Position();
    auto height = norm * (dist * norm);
    if(IsFloatZero(height.Length()))
    {
        return new CollisionData(false);
    }
    auto cosin = height*photon.Direction() / (height.Length()) / photon.Direction().Length();
    if(cosin <= 0)
        return new CollisionData(false);
    auto intersecPoint = photon.Position() + photon.Direction().Normalized() * (height.Length() / cosin);
    auto v1 = p1_ - intersecPoint;
    auto v2 = p2_ - intersecPoint;
    auto v3 = p3_ - intersecPoint;
    auto n1 = v1^v2;
    auto n2 = v2^v3;
    auto n3 = v3^v1;
    if(n1*n2 > 0 && n2*n3 > 0 && n3*n1 > 0)
    {
        if(height * norm < 0)
            return new CollisionData(true, outterMaterial_->GetSelfColor(), intersecPoint, norm);
        else
            return new CollisionData(true, innerMaterial_->GetSelfColor(), intersecPoint, norm);
    }
    return new CollisionData(false);
}

void Polygon::SetOutterMaterial(IMaterial* material)
{
    this->outterMaterial_ = material;
}

void Polygon::SetInnerMaterial(IMaterial* material)
{
    this->innerMaterial_ = material;
}

Point Polygon::GetNormal()
{
    return ((p2_ - p1_) ^ (p3_ - p1_)).Normalized();
}
