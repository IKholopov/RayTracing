#include "Scene.h"

#include <iostream>

Scene::Scene(Camera* camera, IView* view, IGeometryHierarchy* hierarchy,
             std::vector<PointLight*> lights, std::vector<ISceneObject*> objects, std::vector<IMaterial*> materials):camera_(camera), view_(view),
    hierarchy_(hierarchy), lights_(lights), reference_(LightReference(1, 100)), objects_(objects), materials_(materials)
{}

Scene::Scene(Camera* camera, IView* view, IGeometryHierarchy* hierarchy, std::vector<PointLight*> lights,
             LightReference reference, std::vector<ISceneObject*> objects, std::vector<IMaterial*> materials): camera_(camera), view_(view),
    hierarchy_(hierarchy), lights_(lights), reference_(reference), objects_(objects), materials_(materials)
{}

Scene::~Scene()
{
    delete hierarchy_;
    delete camera_;
    for(auto mat: materials_)
        delete mat;
    for(auto obj: objects_)
        delete obj;
    for(auto light: lights_)
        delete light;
}

void RenderPixelTask(Scene* scene,
                 unsigned int x, unsigned int y)
{
    scene->RenderPixel(x, y);
}

void Scene::RenderScene()
{
    std::cout << "Building geometry hierarchy" << std::endl;
    this->hierarchy_->Initialize(this->objects_);
    std::cout << "Rendering" << std::endl;
    ThreadPool pool(std::thread::hardware_concurrency());
    for(unsigned int x = 0; x < camera_->GetWidth(); ++x)
        for(unsigned int y = 0; y < camera_->GetHeight(); ++y)
        {
            pool.AddTask([this, x, y](){
                RenderPixelTask(this, x, y);
            });
        }
    Color* matrix = new Color[view_->GetResolution().Height*view_->GetResolution().Width];
    pool.WaitAll();
    for(unsigned int x = 0; x < camera_->GetWidth(); ++x)
        for(unsigned int y = 0; y < camera_->GetHeight(); ++y)
        {
            pool.AddTask([this, x, y, &matrix](){
                float power = 8;
                Color c = this->view_->GetPixelColor(x, y);
                unsigned int sum = 0;
                if(x > 0)
                {
                    c.R += this->view_->GetPixelColor(x-1, y).R/power;
                    c.G += this->view_->GetPixelColor(x-1, y).G/power;
                    c.B += this->view_->GetPixelColor(x-1, y).B/power;
                    ++sum;
                }
                if(y > 0)
                {
                    c.R += this->view_->GetPixelColor(x, y-1).R/power;
                    c.G += this->view_->GetPixelColor(x, y-1).G/power;
                    c.B += this->view_->GetPixelColor(x, y-1).B/power;
                    ++sum;
                }
                if(x < view_->GetResolution().Width - 1)
                {
                    c.R += this->view_->GetPixelColor(x+1, y).R/power;
                    c.G += this->view_->GetPixelColor(x+1, y).G/power;
                    c.B += this->view_->GetPixelColor(x+1, y).B/power;
                    ++sum;
                }
                if(y < view_->GetResolution().Height - 1)
                {
                    c.R += this->view_->GetPixelColor(x, y+1).R/power;
                    c.G += this->view_->GetPixelColor(x, y+1).G/power;
                    c.B += this->view_->GetPixelColor(x, y+1).B/power;
                    ++sum;
                }
                if(x > 0 && y > 0)
                {
                    c.R += this->view_->GetPixelColor(x-1, y-1).R/power;
                    c.G += this->view_->GetPixelColor(x-1, y-1).G/power;
                    c.B += this->view_->GetPixelColor(x-1, y-1).B/power;
                    ++sum;
                }
                if(y > 0 && x < view_->GetResolution().Width)
                {
                    c.R += this->view_->GetPixelColor(x+1, y-1).R/power;
                    c.G += this->view_->GetPixelColor(x+1, y-1).G/power;
                    c.B += this->view_->GetPixelColor(x+1, y-1).B/power;
                    ++sum;
                }
                if(x < view_->GetResolution().Width - 1 && y < view_->GetResolution().Height - 1)
                {
                    c.R += this->view_->GetPixelColor(x+1, y+1).R/power;
                    c.G += this->view_->GetPixelColor(x+1, y+1).G/power;
                    c.B += this->view_->GetPixelColor(x+1, y+1).B/power;
                    ++sum;
                }
                if(y < view_->GetResolution().Height - 1 && x > 0)
                {
                    c.R += this->view_->GetPixelColor(x-1, y+1).R/power;
                    c.G += this->view_->GetPixelColor(x-1, y+1).G/power;
                    c.B += this->view_->GetPixelColor(x-1, y+1).B/power;
                    ++sum;
                }
                c.R /= float(sum)/power + 1;
                c.G /= float(sum)/power + 1;
                c.B /= float(sum)/power + 1;
                matrix[(view_->GetResolution().Width*y + x)] = c;
            });
        }
    pool.WaitAll();
    for(unsigned int x = 0; x < camera_->GetWidth(); ++x)
        for(unsigned int y = 0; y < camera_->GetHeight(); ++y)
        {
            pool.AddTask([this, x, y, &matrix](){
                this->view_->UpdatePixel(x,y, matrix[(view_->GetResolution().Width*y + x)]);
            });
        }
    pool.WaitAll();
    pool.Terminate();
    delete[] matrix;
    std::cout << "Finished rendering" << std::endl;
}

void Scene::RenderPixel(unsigned int x, unsigned int y)
{
    auto photon = camera_->GetPhotonForPixel(x, y);
    auto collision = hierarchy_->RenderPhoton(photon);
    if(!collision->IsCollide)
    {
        view_->UpdatePixel(x, y, Color(0, 0, 0));
        return;
    }
    collision->ReflectionDepth = 0;
    collision->RefractionDepth = 0;
    collision->PhotonDirection = photon.Direction();
    collision->Material->RenderMaterial(*hierarchy_, collision, this->lights_, config_, reference_);
    view_->UpdatePixel(x, y, collision->PixelColor);
}

void Scene::SetView(IView* view)
{
    this->view_ = view;
    this->camera_->SetResolution(view->GetResolution());
}

void Scene::SetConfig(RenderConfig config)
{
    this->config_ = config;
}

const std::vector<ISceneObject*>& Scene::GetGeometry() const
{
    return this->objects_;
}

const std::vector<PointLight*>&Scene::GetLights() const
{
    return this->lights_;
}

const std::vector<IMaterial*>&Scene::GetMaterials() const
{
    return this->materials_;
}

const Camera* Scene::GetCamera() const
{
    return this->camera_;
}

const LightReference&Scene::GetLightReference() const
{
    return this->reference_;
}

Color Scene::EmitLights(CollisionData& collision)
{
    Color c = collision.PixelColor.RGBtoHSV();
    c.B = 0;
    for(auto light: lights_)
    {
        Photon photon(collision.CollisionPoint, light->GetPosition() - collision.CollisionPoint, collision.Owner);
        if(hierarchy_->RenderPhoton(photon)->IsCollide)
            continue;
        Color lColor = light->GetLight().RGBtoHSV();
        lColor.B *= (light->GetIntensity() / std::pow((light->GetPosition() - collision.CollisionPoint).Length(),2))*
                (collision.CollisionNormal.Normalized()*photon.Direction().Normalized());
        c = c + lColor;
    }
    collision.PixelColor = c.HSVtoRGB();
    return collision.PixelColor;
}
