#include "SimpleSerializer.h"

#include <fstream>
#include <iostream>

#include "KDTree.h"
#include "Sphere.h"
#include "Polygon.h"
#include "Parallelogram.h"
#include "SimpleMaterial.h"

void errorFile()
{
    std::cerr << "Corrupted scene file" << std::endl;
    exit(1);
}

Scene* SimpleSerializer::LoadScene(std::__cxx11::string filepath, IView* view)
{
    std::ifstream stream(filepath.c_str());
    if(!stream.is_open())
    {
        std::cerr << "Can't open scene file " << filepath << std::endl;
        exit(1);
    }
    Camera* camera = nullptr;
    std::vector<ISceneObject*> objects;
    std::string s;
    bool sRead = false;
    while(!stream.eof())
    {
        if(!sRead)
            if(!(stream >> s))
                errorFile();
        sRead = false;
        if(!s.compare("camera")){
            unsigned int width, height;
            Point viewpoint, p1, p2, p3, p4;
            stream >> viewpoint;
            stream >> width;
            stream >> height;
            stream >> p1;
            stream >> p2;
            stream >> p3;
            stream >> p4;
            if(camera != nullptr)
            {
                std::cerr << "Too many cameras" << std::endl;
                delete camera;
            }
            camera = new Camera(viewpoint, width, height, Plane(p1, p2, p3, p4));
        }
        else if(!s.compare("sphere"))
        {
            Point position;
            float radius;
            stream >> position;
            stream >> radius;
            Sphere* sphere = new Sphere(radius, position, nullptr);
            stream >> s;
            if(!s.compare("outMaterial"))
            {
                IMaterial* material = LoadMaterial(stream);
                sphere->SetOutterMaterial(material);
            }
            else{
                sRead = true;
                objects.push_back(sphere);
                continue;
            }
            stream >> s;
            if(!s.compare("inMaterial"))
            {
                IMaterial* material = LoadMaterial(stream);
                sphere->SetInnerMaterial(material);
            }
            else{
                sRead = true;
                objects.push_back(sphere);
                continue;
            }
            objects.push_back(sphere);
        }
        else if(!s.compare("polygon"))
        {
            Point p1, p2, p3;
            stream >> p1 >> p2 >> p3;
            Polygon* pol = new Polygon(p1 ,p2, p3);
            stream >> s;
            if(!s.compare("outMaterial"))
            {
                IMaterial* material = LoadMaterial(stream);
                pol->SetOutterMaterial(material);
            }
            else{
                sRead = true;
                objects.push_back(pol);
                continue;
            }
            stream >> s;
            if(!s.compare("inMaterial"))
            {
                IMaterial* material = LoadMaterial(stream);
                pol->SetInnerMaterial(material);
            }
            else{
                sRead = true;
                objects.push_back(pol);
                continue;
            }
            objects.push_back(pol);
        }
        else if(!s.compare("parallelogram"))
        {
            Point pivot, left, right;
            stream >> pivot >> left >> right;
            Parallelogram * pal = new Parallelogram(pivot , left, right);
            stream >> s;
            if(!s.compare("outMaterial"))
            {
                IMaterial* material = LoadMaterial(stream);
                pal->SetOutterMaterial(material);
            }
            else{
                sRead = true;
                objects.push_back(pal);
                continue;
            }
            stream >> s;
            if(!s.compare("inMaterial"))
            {
                IMaterial* material = LoadMaterial(stream);
                pal->SetInnerMaterial(material);
            }
            else{
                sRead = true;
                objects.push_back(pal);
                continue;
            }
            objects.push_back(pal);
        }
    }
    auto tree = new KDTree(1.0, 1.0);
    tree->Initialize(objects);
    return new Scene(*camera, view, tree);
}

IMaterial* SimpleSerializer::LoadMaterial(std::ifstream& stream)
{
    std::string s;
    stream >> s;
    if(!s.compare("SimpleMaterial"))
    {
        Color color;
        stream >> color.R;
        stream >> color.G;
        stream >> color.B;
        return new SimpleMaterial(color);
    }
    else
    {
        throw std::exception();
    }
}
