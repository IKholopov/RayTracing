#include "SimpleMaterial.h"

SimpleMaterial::SimpleMaterial(Color color):color_(color), alpha_(0)
{

}

SimpleMaterial::SimpleMaterial(Color color, float alpha):color_(color),
    alpha_(alpha)
{

}

Color SimpleMaterial::RenderMaterial(IGeometryHierarchy& hierarchy,
                                   CollisionData* data, std::vector<PointLight*>& lights)
{
    data->PixelColor = data->PixelColor.RGBtoHSV();
    data->PixelColor.B = 0;
    if(IsFloatZero(this->alpha_) || data->Depth > 2)
    {
        for(auto light: lights)
            light->EmitLight(*data, hierarchy);
        data->PixelColor = data->PixelColor.HSVtoRGB();
            data->PixelColor = data->PixelColor.AddWithIntensivity(data->Material->GetSelfColor(), 0.2);
        return data->PixelColor;
    }
    data->PixelColor = data->PixelColor.AddWithIntensivity(data->Material->GetSelfColor(), 0.2);
    Photon reflectionPhoton(data->CollisionPoint, data->PhotonDirection*(-1) +
                                  (data->PhotonDirection - data->CollisionNormal*
                                   (data->PhotonDirection*data->CollisionNormal.Normalized()))* 2, data->Owner);
    auto reflection = hierarchy.RenderPhoton(reflectionPhoton);
    reflection->Depth = data->Depth + 1;
    if(reflection->IsCollide)
    {
        reflection->PhotonDirection = reflectionPhoton.Direction();
        reflection->Material->RenderMaterial(hierarchy, reflection, lights);
        auto originalColor  = reflection->PixelColor;
        reflection->PixelColor = reflection->PixelColor.RGBtoHSV();
        reflection->PixelColor.B = 0;
        for(auto light: lights)
            light->EmitLight(*reflection, hierarchy);
        reflection->PixelColor = reflection->PixelColor.HSVtoRGB();
        reflection->PixelColor = reflection->PixelColor.AddWithIntensivity(originalColor, 0.2);
    }
    else
        reflection->PixelColor = Color(0, 0, 0);
    data->PixelColor = reflection->PixelColor*alpha_ + GetSelfColor()*(1 - alpha_);
    return data->PixelColor;
}

Color SimpleMaterial::GetSelfColor()
{
    return color_;
}
