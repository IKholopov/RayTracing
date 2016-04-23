#include "Kernel.h"

#include "KDTree.h"
#include "Sphere.h"
#include "SimpleMaterial.h"

void launch(Kernel* kernel)
{
    kernel->RunScene();
}

Kernel::Kernel(IView* view):view_(view)
{
}

Kernel::Kernel(IView* view, ISerializer* serializer, std::__cxx11::string sceneFilePath):view_(view)
{
    scene_ = serializer->LoadScene(sceneFilePath, view_);
}

Kernel::~Kernel()
{
    delete scene_;
}

void Kernel::Run()
{
    thr = new std::thread(launch, this);
}

void Kernel::RunScene()
{
    scene_->RenderScene();    
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
