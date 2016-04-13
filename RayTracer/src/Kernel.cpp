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
    /*std::vector<ISceneObject*> objects;
    SimpleMaterial* blue = new SimpleMaterial(Color(0.0f, 0.0f, 1.0f));
    SimpleMaterial* red = new SimpleMaterial(Color(1.0f, 0.0f, 0.0f));
    auto sphere = new Sphere(2, Point(0, 60, 0), blue);
    auto sphere2 = new Sphere(2, Point(6, 60, 0), red);
    //auto sphere3 = new Sphere(1, Point(-10, 50, 0), material);
    objects.push_back(sphere);
    objects.push_back(sphere2);
    //objects.push_back(sphere3);
    IGeometryHierarchy* tree = new KDTree(1.0, 1.0);
    tree->Initialize(objects);
    scene_ = new Scene(Camera(Point(0,-10, 0), 640, 480,
                         Plane(Point(-3, 1, 2), Point(3, 1, 2), Point(-3, 1, -2), Point(3, 1, -2))), view_, tree);*/

    scene_->RenderScene();

}
