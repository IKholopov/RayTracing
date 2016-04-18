#include "Scene.h"

Scene::Scene(Camera camera, IView* view, IGeometryHierarchy* hierarchy, std::vector<PointLight*> lights):camera_(camera), view_(view),
    hierarchy_(hierarchy), lights_(lights)
{}

void RenderPixel(Camera& camera, IGeometryHierarchy* hierarchy, IView* view, std::vector<PointLight*>& lights,
                 unsigned int x, unsigned int y)
{
    //x = 170;
    //y = 105;
    //x = 450;
    //y = 105;
    auto photon = camera.GetPhotonForPixel(x, y);
    auto color = hierarchy->RenderPhoton(photon, lights);
    view->UpdatePixel(x, y, color);
    //view->UpdatePixel(450, 105, Color(1, 0, 0));
}

void Scene::RenderScene()
{
    ThreadPool pool(std::thread::hardware_concurrency());
    for(unsigned int x = 0; x < camera_.GetWidth(); ++x)
        for(unsigned int y = 0; y < camera_.GetHeight(); ++y)
        {
            pool.AddTask([this, x, y](){
                RenderPixel(this->camera_, hierarchy_, view_, lights_, x, y);
            });
        }
    Color matrix[view_->GetResolution().Height*view_->GetResolution().Width];
    pool.WaitAll();
    for(unsigned int x = 0; x < camera_.GetWidth(); ++x)
        for(unsigned int y = 0; y < camera_.GetHeight(); ++y)
        {
            pool.AddTask([this, x, y, &matrix](){
                float power = 6;
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
    for(unsigned int x = 0; x < camera_.GetWidth(); ++x)
        for(unsigned int y = 0; y < camera_.GetHeight(); ++y)
        {
            pool.AddTask([this, x, y, &matrix](){
                this->view_->UpdatePixel(x,y, matrix[(view_->GetResolution().Width*y + x)]);
            });
        }
    pool.Terminate();
    int x = 170;
    int y = 105;
    this->view_->UpdatePixel(x,y, Color(1, 0, 0));
}
