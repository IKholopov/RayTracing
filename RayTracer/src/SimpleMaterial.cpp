#include "SimpleMaterial.h"

SimpleMaterial::SimpleMaterial(Color color):color_(color), alpha_(0)
{

}

SimpleMaterial::SimpleMaterial(Color color, float alpha, float n):color_(color),
    alpha_(alpha), n_(n)
{

}

Color SimpleMaterial::RenderMaterial(IGeometryHierarchy& hierarchy,
                                   CollisionData* data, std::vector<PointLight*>& lights, RenderConfig& config, LightReference& reference)
{
    float passive = 0;
    if(!IsFloatZero(this->n_) && config.refraction() && data->RefractionDepth < config.refractionDepth())
    {
        Photon refractPhoton(data->CollisionPoint, data->CollisionNormal * (data->PhotonDirection*data->CollisionNormal) +
                             (data->PhotonDirection - data->CollisionNormal * (data->PhotonDirection*data->CollisionNormal.Normalized()))*
                             (1.0/this->n_), data->Owner);
        auto refract = hierarchy.RenderPhoton(refractPhoton);
        refract->RefractionDepth = data->RefractionDepth + 1;
        if(refract->IsCollide)
        {
            refract->PhotonDirection = refractPhoton.Direction();
            refract->Material->RenderMaterial(hierarchy, refract, lights, config, reference);
            data->PixelColor = refract->PixelColor;
        }
        else
            data->PixelColor = Color(0, 0, 0);
        delete refract;
    }
    else
    {
        data->PixelColor = data->PixelColor.RGBtoHSV();
        if(IsFloatZero(this->alpha_) || !config.reflection()|| data->ReflectionDepth > config.reflectionDepth())
        {
                if(config.light())
                {
                    data->PixelColor.B = 0;
                    for(auto light: lights)
                        light->EmitLight(*data, hierarchy, reference, this->GetSelfColor());
                    data->PixelColor = data->PixelColor.HSVtoRGB();
                    data->PixelColor = data->PixelColor.AddWithIntensivity(data->Material->GetSelfColor(), passive);
                }
                else
                    data->PixelColor = data->PixelColor.HSVtoRGB();
        }
        else
        {
            data->PixelColor = data->PixelColor.AddWithIntensivity(data->Material->GetSelfColor(), passive);
            data->PixelColor = data->PixelColor.HSVtoRGB();
        }
    }
    if(this->alpha_ > 0 && config.reflection())
    {
        Photon reflectionPhoton(data->CollisionPoint, data->PhotonDirection*(-1) +
                                      (data->PhotonDirection - data->CollisionNormal*
                                       (data->PhotonDirection*data->CollisionNormal.Normalized()))* 2, data->Owner);
        auto reflection = hierarchy.RenderPhoton(reflectionPhoton);
        reflection->ReflectionDepth = data->ReflectionDepth + 1;
        if(reflection->IsCollide)
        {
            reflection->PhotonDirection = reflectionPhoton.Direction();
            reflection->Material->RenderMaterial(hierarchy, reflection, lights, config, reference);
            auto originalColor  = reflection->PixelColor;
            reflection->PixelColor = reflection->PixelColor.AddWithIntensivity(originalColor, passive);
        }
        else
            reflection->PixelColor = Color(0, 0, 0);
        data->PixelColor = reflection->PixelColor*alpha_ + data->PixelColor*(1 - alpha_);
        delete reflection;
    }
    return data->PixelColor;
}

Color SimpleMaterial::GetSelfColor()
{
    return color_;
}

float SimpleMaterial::GetAlpha() const
{
    return this->alpha_;
}

float SimpleMaterial::GetN() const
{
    return this->n_;
}
