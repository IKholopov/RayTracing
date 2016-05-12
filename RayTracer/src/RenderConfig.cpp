#include "RenderConfig.h"

RenderConfig::RenderConfig():light_(true), reflection_(true), refraction_(true), reflectionDepth_(10)
{
}

unsigned int RenderConfig::reflectionDepth() const
{
    return reflectionDepth_;
}

void RenderConfig::setReflectionDepth(unsigned int reflectionDepth)
{
    reflectionDepth_ = reflectionDepth;
}

bool RenderConfig::refraction() const
{
    return refraction_;
}

void RenderConfig::setRefraction(bool refraction)
{
    refraction_ = refraction;
}

bool RenderConfig::reflection() const
{
    return reflection_;
}

void RenderConfig::setReflection(bool reflection)
{
    reflection_ = reflection;
}

bool RenderConfig::light() const
{
    return light_;
}

void RenderConfig::setLight(bool light)
{
    light_ = light;
}
