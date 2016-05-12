#include "Photon.h"
#include "limits"

Photon::Photon(Point position, Point direction, ISceneObject* owner):\
    position_(position), direction_(direction), minDist_(-1), owner_(owner)
{}

Photon::Photon(Point position, Point direction, float minDist, ISceneObject* owner):
    position_(position), direction_(direction), minDist_(minDist), owner_(owner)
{}

Point Photon::Position() const
{
    return position_;
}

Point Photon::Direction() const
{
    return direction_;
}

float Photon::GetMinDist() const
{
    return minDist_;
}

ISceneObject*Photon::Owner() const
{
    return owner_;
}


bool Photon::IntersecWithBox(const Box& box) const
{
    float low, high, tNear, tFar;
    if(!IsFloatZero(this->Direction().X))
    {
        low = 1.0f/this->Direction().X*(box.XMin - this->position_.X);
        high = 1.0f/this->Direction().X*(box.XMax - this->position_.X);
        tNear = std::min(low, high);
        tFar = std::max(low, high);
    }
    else
    {
        tNear = std::numeric_limits<float>::min();
        tFar = std::numeric_limits<float>::max();
    }
    if(!IsFloatZero(this->Direction().Y))
    {
        low = 1.0f/this->Direction().Y*(box.YMin - this->position_.Y);
        high = 1.0f/this->Direction().Y*(box.YMax - this->position_.Y);
        tNear = std::max(tNear, std::min(low, high));
        tFar = std::min(tFar, std::max(low, high));
    }
    if(!IsFloatZero(this->Direction().Z))
    {
        low = 1.0f/this->Direction().Z*(box.ZMin - this->position_.Z);
        high = 1.0f/this->Direction().Z*(box.ZMax - this->position_.Z);
        tNear = std::max(tNear, std::min(low, high));
        tFar = std::min(tFar, std::max(low, high));
    }
    return (tFar >= tNear) && (tFar > 0.0f);
}
