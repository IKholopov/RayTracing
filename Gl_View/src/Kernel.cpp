#include "Kernel.h"

#include "GLView.h"
#include "KDTree.h"
#include "Sphere.h"
#include "SimpleMaterial.h"

void launch(Kernel* kernel)
{
    kernel->RunScene();
}

Kernel::Kernel()
{
    view_ = new GLView(640, 480);
}

Kernel::~Kernel()
{

}

void Kernel::Run()
{
    thr = new std::thread(launch, this);
}

void Kernel::RunScene()
{
    std::vector<ISceneObject*> objects;
    SimpleMaterial* material = new SimpleMaterial(Color(0.0f, 0.0f, 1.0f));
    auto sphere = new Sphere(1, Point(1, 50, 0), material);
    auto sphere2 = new Sphere(1, Point(10, 50, 0), material);
    auto sphere3 = new Sphere(1, Point(-10, 50, 0), material);
    objects.push_back(sphere);
    objects.push_back(sphere2);
    objects.push_back(sphere3);
    IGeometryHierarchy* tree = new KDTree(1.0, 1.0);
    tree->Initialize(objects);
    auto scene = Scene(Camera(Point(0,-10, 0), 640, 480,
                         Plane(Point(-3, 1, 2), Point(3, 1, 2), Point(-3, 1, -2), Point(3, 1, -2))), view_, tree);
    scene.RenderScene();

}

IView*Kernel::GetView()
{
    return view_;
}
