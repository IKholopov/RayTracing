#include "Scene.h"

Scene::Scene(Camera camera, IView* view, IGeometryHierarchy* hierarchy):camera_(camera), view_(view), hierarchy_(hierarchy)
{}

void Scene::RenderScene()
{
    for(unsigned int x = 0; x < camera_.GetWidth(); ++x)
        for(int y = 0; y < camera_.GetHeight(); ++y)
        {
            auto photon = camera_.GetPhotonForPixel(x, y);
            if(x == 430 && y == 140)
                int a = 0;
            auto color = hierarchy_->RenderPhoton(photon);
            view_->UpdatePixel(x, y, color);
        }
}
