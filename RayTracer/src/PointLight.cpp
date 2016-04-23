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
