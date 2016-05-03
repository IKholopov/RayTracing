#include "PointLight.h"

PointLight::PointLight(Point position, Color light, float intensity):position_(position), light_(light), intensity_(intensity)
{
}

Point PointLight::GetPosition()
{
    return position_;
}

Color PointLight::GetLight()
{
    return light_;
}

float PointLight::GetIntensity()
{
    return intensity_;
}

Color PointLight::EmitLight(CollisionData& collision, IGeometryHierarchy& hierarchy)
{
    Color c = collision.PixelColor;
    Photon photon(collision.CollisionPoint, this->GetPosition() - collision.CollisionPoint,
                  (this->GetPosition() - collision.CollisionPoint).Length(), collision.Owner);
    auto collData = hierarchy.RenderPhoton(photon);
    if(collData->IsCollide && (this->GetPosition() - collision.CollisionPoint).Length() <=
            (collData->CollisionPoint - collision.CollisionPoint).Length())
    {
        delete collData;
        return collision.PixelColor;
    }
    delete collData;
    Color lColor = this->GetLight().RGBtoHSV();
    lColor.B *= (this->GetIntensity() / std::pow((this->GetPosition() - collision.CollisionPoint).Length(),2))*
        (collision.CollisionNormal.Normalized()*photon.Direction().Normalized());

    c = c + lColor;
    collision.PixelColor = c;
    return collision.PixelColor;
}
