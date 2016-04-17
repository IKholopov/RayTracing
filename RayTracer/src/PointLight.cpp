#include "PointLight.h"

PointLight::PointLight(Point position, Color light):position_(position), light_(light)
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
