#include "Parallelogram.h"

Parallelogram::Parallelogram(Point pivot, Point left, Point right,
                             IMaterial* outterMaterial, IMaterial* innerMaterial): pivot_(pivot),left_(left), right_(right),
                            outterMaterial_(outterMaterial), innerMaterial_(innerMaterial)
{
    float XMax, YMax, ZMax, XMin, YMin, ZMin;
    Point p1_ = pivot;
    Point p2_ = left;
    Point p3_ = left + (right - pivot);
    Point p4_ = right;
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

Box Parallelogram::GetBoundingBox()
{
    return boundingBox_;
}

CollisionData* Parallelogram::GetCollision(Photon photon)
{
    auto norm = GetNormal();
    auto dist = pivot_ - photon.Position();
    auto height = norm * (dist * norm);
    if(IsFloatZero(height.Length()))
    {
        return new CollisionData(false);
    }
    auto cosin = height*photon.Direction() / (height.Length()) / photon.Direction().Length();
    if(cosin <= 0)
        return new CollisionData(false);
    auto intersecPoint = photon.Position() + photon.Direction().Normalized() * (height.Length() / cosin);
    auto v1 = pivot_ - intersecPoint;
    auto v2 = left_ - intersecPoint;
    auto v3 = left_ + (right_ - pivot_) - intersecPoint;
    auto v4 = right_ - intersecPoint;
    auto n1 = v1^v2;
    auto n2 = v2^v3;
    auto n3 = v3^v4;
    auto n4 = v4^v1;
    if(n1*n2 > 0 && n2*n3 > 0 && n3*n4 > 0 && n4*n1 > 0)
    {
        if(height * norm < 0)
            return new CollisionData(true, outterMaterial_->GetSelfColor(), intersecPoint, norm);
        else
            return new CollisionData(true, innerMaterial_->GetSelfColor(), intersecPoint, norm);
    }
    return new CollisionData(false);
}

void Parallelogram::SetOutterMaterial(IMaterial* material)
{
    this->outterMaterial_ = material;
}

void Parallelogram::SetInnerMaterial(IMaterial* material)
{
    this->innerMaterial_ = material;
}

Point Parallelogram::GetNormal()
{
    return ((right_ - pivot_) ^ (left_ - pivot_)).Normalized();
}
