#include "Scene.h"

Scene::Scene(Camera camera, IView* view, IGeometryHierarchy* hierarchy):camera_(camera), view_(view), hierarchy_(hierarchy)
{}

void RenderPixel(Camera& camera, IGeometryHierarchy* hierarchy, IView* view, unsigned int x, unsigned int y)
{
    auto photon = camera.GetPhotonForPixel(x, y);
    auto color = hierarchy->RenderPhoton(photon);
    view->UpdatePixel(x, y, color);
}

void Scene::RenderScene()
{
    ThreadPool pool(4);
    for(unsigned int x = 0; x < camera_.GetWidth(); ++x)
        for(unsigned int y = 0; y < camera_.GetHeight(); ++y)
        {
            pool.AddTask([this, x, y](){
                RenderPixel(this->camera_, hierarchy_, view_, x, y);
            });
        }
    pool.Terminate();
}
