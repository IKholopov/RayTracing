#include "Kernel.h"

#include "Sphere.h"
#include "SimpleMaterial.h"

void launch(Kernel* kernel)
{
    kernel->RunScene();
}

Kernel::Kernel(IView* view):view_(view), thr(nullptr), sceneStatus_(NotLoaded)
{
}

Kernel::Kernel(IView* view, ISerializer* serializer, std::__cxx11::string sceneFilePath):view_(view), thr(nullptr), sceneStatus_(NotLoaded)
{
    scene_ = serializer->LoadScene(sceneFilePath, view_);
    this->sceneStatus_ = Loaded;
}

Kernel::~Kernel()
{
    if(thr)
    {
        if(sceneStatus_ == Rendering)
            thr->join();
        delete thr;
    }

    delete scene_;
}

void Kernel::Run()
{
    thr = new std::thread(launch, this);
    sceneStatus_ = Rendering;
}

void Kernel::RunScene()
{
    scene_->RenderScene();
}

void Kernel::WaitForRender()
{
    thr->join();
    sceneStatus_ = Rendered;
}

void Kernel::ExportScene(ISerializer* serializer, std::__cxx11::string sceneFilePath)
{
    serializer->ExportScene(sceneFilePath, this->scene_);
}

void Kernel::LoadScene(ISerializer* serializer, std::__cxx11::string sceneFilePath)
{
    scene_ = serializer->LoadScene(sceneFilePath, view_);
}

void Kernel::SetView(IView* view)
{
    this->view_ = view;
    this->scene_->SetView(view);
}

void Kernel::SetConfig(RenderConfig config)
{
    this->scene_->SetConfig(config);
}
