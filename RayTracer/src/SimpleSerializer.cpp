#include "SimpleSerializer.h"

#include <fstream>
#include <iostream>

//#include "KDTree.h"
//#include "SHATree.h"
//#include "SHAKDTree.h"
#include "KDFairTree.h"
#include "Sphere.h"
#include "Polygon.h"
#include "Parallelogram.h"
#include "Quadrangle.h"
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
    std::vector<IMaterial*> materials;
    std::vector<PointLight*> lights;
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
                materials.push_back(material);
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
                materials.push_back(material);
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
                materials.push_back(material);
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
                materials.push_back(material);
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
                materials.push_back(material);
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
                materials.push_back(material);
                pal->SetInnerMaterial(material);
            }
            else{
                sRead = true;
                objects.push_back(pal);
                continue;
            }
            objects.push_back(pal);
        }
        else if(!s.compare("quadrangle"))
        {
            Point p1, p2, p3, p4;
            stream >> p1 >> p2 >> p3 >> p4;
            Quadrangle* quad = new Quadrangle(p1, p2, p3, p4);
            stream >> s;
            if(!s.compare("outMaterial"))
            {
                IMaterial* material = LoadMaterial(stream);
                materials.push_back(material);
                quad->SetOutterMaterial(material);
            }
            else{
                sRead = true;
                objects.push_back(quad);
                continue;
            }
            stream >> s;
            if(!s.compare("inMaterial"))
            {
                IMaterial* material = LoadMaterial(stream);
                materials.push_back(material);
                quad->SetInnerMaterial(material);
            }
            else{
                sRead = true;
                objects.push_back(quad);
                continue;
            }
            objects.push_back(quad);
        }
        else if(!s.compare("pointLight"))
        {
            Point p;
            Color light;
            float intensity;
            stream >> p;
            stream >> light.R >> light.G >> light.B;
            stream >> intensity;
            lights.push_back(new PointLight(p, light, intensity));
        }
    }
    auto tree = new KDFairTree(10);
    //tree->Initialize(objects);
    return new Scene(camera, view, tree, lights, objects, materials);
}

void SimpleSerializer::ExportScene(std::__cxx11::string filepath, Scene* scene)
{

}

IMaterial* SimpleSerializer::LoadMaterial(std::ifstream& stream)
{
    std::string s;
    stream >> s;
    if(!s.compare("SimpleMaterial"))
    {
        Color color;
        float alpha;
        stream >> color.R;
        stream >> color.G;
        stream >> color.B;
        stream >> alpha;
        return new SimpleMaterial(color, alpha / 100, 0);
    }
    else
    {
        throw std::exception();
    }
}
