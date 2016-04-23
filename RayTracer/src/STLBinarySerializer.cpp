#include "STLBinarySerializer.h"

#include <stdio.h>
#include <iostream>

#include "KDTree.h"
#include "SHATree.h"
#include "SHAKDTree.h"
#include "Sphere.h"
#include "Polygon.h"
#include "Parallelogram.h"
#include "SimpleMaterial.h"

Scene* STLBinarySerializer::LoadScene(std::__cxx11::string filepath, IView* view)
{
    FILE* f;
    if(!(f = fopen(filepath.c_str(), "r")))
    {
        std::cerr << "Can't open scene file " << filepath << std::endl;
        exit(1);
    }
    Camera* camera = new Camera(Point(0,-10, 0), 640, 480,
                                Plane(Point(-3, 1, 2), Point(3, 1, 2),
                                Point(-3, 1, -2), Point(3, 1, -2)));
    std::vector<ISceneObject*> objects;
    std::vector<PointLight*> lights;
    lights.push_back(new PointLight(Point(2, 0, 10), Color(1, 1, 1), 100));
    lights.push_back(new PointLight(Point(-10, -10, 20), Color(1, 1, 1), 100));
    lights.push_back(new PointLight(Point(-10, 40, 20), Color(1, 1, 1), 100));
    char title[80];
    int nFaces;
    fread(title, 80, 1, f);
    fread((void*)&nFaces, 4, 1, f);
    float v[12];
    unsigned short uint16;
    for (size_t i=0; i<nFaces; ++i) {
                for (size_t j=0; j<12; ++j) {
                    fread((void*)&v[j], sizeof(float), 1, f);
                }
                fread((void*)&uint16, sizeof(unsigned short), 1, f);
                auto obj = new Polygon(Point(v[3], v[4]+20, v[5]-5), Point(v[6], v[7]+20, v[8]-5), Point(v[9], v[10]+20, v[11]-5));
                obj->SetInnerMaterial(new SimpleMaterial(Color(0,0,1)));
                obj->SetOutterMaterial(new SimpleMaterial(Color(0,0,1)));
                objects.push_back(obj);
    }
    fclose(f);
    auto tree = new SHAKDTree(10.0, 1.0, 2);//KDTree(10.0, 1.0);//
    tree->Initialize(objects);
    return new Scene(*camera, view, tree, lights);
}
