#include "CustomSerializer.h"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

//#include "KDTree.h"
//#include "SHATree.h"
//#include "SHAKDTree.h"
#include "STLBinarySerializer.h"
#include "KDFairTree.h"
#include "Sphere.h"
#include "Polygon.h"
#include "Quadrangle.h"
#include "Parallelogram.h"
#include "SimpleMaterial.h"

Scene* CustomSerializer::LoadScene(std::__cxx11::string filepath, IView* view)
{
    std::ifstream stream(filepath.c_str());
    if(!stream.is_open())
    {
        std::cerr << "Can't open scene file " << filepath << std::endl;
        exit(1);
    }
    Camera* camera;
    std::vector<ISceneObject*> objects;
    std::vector<PointLight*> lights;
    LightReference reference(0, 0);
    char s[4097];
    std::string str;
    std::unordered_map<std::string, IMaterial*> materials;
    while(!stream.eof())
    {
        stream.getline(s, 4096);
        std::stringstream line(s);
        line >> str;
        if(str[0] == '#')
            continue;
        if(!str.compare("viewport"))
        {
            bool finishedPort = false;
            Point origin, topLeft, bottomLeft, topRight;
            while(!stream.eof())
            {
                stream.getline(s, 4096);
                std::stringstream l(s);
                l >> str;
                if(str[0] == '#')
                    continue;
                if(!str.compare("origin"))
                {
                    float x, y, z;
                    l >> x >> y >> z;
                    origin = Point(x, y, z);
                }
                else if(!str.compare("topleft"))
                {
                    float x, y, z;
                    l >> x >> y >> z;
                    topLeft = Point(x, y, z);
                }
                else if(!str.compare("topright"))
                {
                    float x, y, z;
                    l >> x >> y >> z;
                    topRight = Point(x, y, z);
                }
                else if(!str.compare("bottomleft"))
                {
                    float x, y, z;
                    l >> x >> y >> z;
                    bottomLeft = Point(x, y, z);
                }
                else if(!str.compare("endviewport"))
                {
                    camera = new Camera(origin,
                                        Plane(topLeft, topRight, bottomLeft, bottomLeft+(topRight-topLeft)));
                    finishedPort = true;
                    break;
                }
            }
            if(!finishedPort)
                std::cerr << "Invalid file format " << filepath << std::endl;
        }
        else if(!str.compare("materials"))
        {
            bool finishedMaterials = false;
            while(!stream.eof())
            {
                stream.getline(s, 4096);
                std::stringstream l(s);
                l >> str;
                if(str[0] == '#')
                    continue;
                if(!str.compare("entry"))
                {
                    bool finishedEntry = false;
                    unsigned int R, G, B;
                    float reflect;
                    float refract;
                    std::string name;
                    Color color;
                    while(!stream.eof())
                    {
                        stream.getline(s, 4096);
                        std::stringstream l(s);
                        l >> str;
                        if(str[0] == '#')
                            continue;
                        if(!str.compare("name"))
                            l >> name;
                        else if(!str.compare("color"))
                        {
                            l >> R >> G >> B;
                            color = Color(R/255.0, G/255.0, B/255.0);
                        }
                        else if(!str.compare("reflect"))
                            l >> reflect;
                        else if(!str.compare("refract"))
                            l >> refract;
                        else if(!str.compare("endentry"))
                        {
                            finishedEntry = true;
                            materials.insert(std::pair<std::string, IMaterial*>(name,
                                        new SimpleMaterial(color, reflect, refract)));
                            break;
                        }
                    }
                    if(!finishedEntry)
                    {
                        std::cerr << "Invalid file format " << filepath << std::endl;
                        exit(1);
                    }
                }
                else if(!str.compare("endmaterials"))
                {
                    finishedMaterials = true;
                    break;
                }
            }
            if(!finishedMaterials)
            {
                std::cerr << "Invalid file format " << filepath << std::endl;
                exit(1);
            }
        }
        else if(!str.compare("lights"))
        {
            bool finishedLights= false;
            while(!stream.eof())
            {
                stream.getline(s, 4096);
                std::stringstream l(s);
                l >> str;
                if(str[0] == '#')
                    continue;
                if(!str.compare("reference"))
                {
                    bool finishedReference = false;
                    float p;
                    float dist;
                    while(!stream.eof())
                    {
                        stream.getline(s, 4096);
                        std::stringstream l(s);
                        l >> str;
                        if(str[0] == '#')
                            continue;
                        if(!str.compare("power"))
                            l >> p;
                        else if(!str.compare("distance"))
                            l >> dist;
                        else if(!str.compare("endreference"))
                        {
                            finishedReference = true;
                            reference = LightReference(p, dist);
                            break;
                        }
                    }
                    if(!finishedReference)
                    {
                        std::cerr << "Invalid file format " << filepath << std::endl;
                        exit(1);
                    }
                }
                else if(!str.compare("point"))
                {
                    bool finishedPoint = false;
                    Point pos;
                    float p;
                    while(!stream.eof())
                    {
                        stream.getline(s, 4096);
                        std::stringstream l(s);
                        l >> str;
                        if(str[0] == '#')
                            continue;
                        if(!str.compare("coords"))
                            l >> pos;
                        else if(!str.compare("power"))
                            l >> p;
                        else if(!str.compare("endpoint"))
                        {
                            finishedPoint = true;
                            lights.push_back(new PointLight(pos, Color(1,1,1), p));
                            break;
                        }
                    }
                    if(!finishedPoint)
                    {
                        std::cerr << "Invalid file format " << filepath << std::endl;
                        exit(1);
                    }
                }
                else if(!str.compare("endlights"))
                {
                    finishedLights = true;
                    break;
                }
            }
            if(!finishedLights)
            {
                std::cerr << "Invalid file format " << filepath << std::endl;
                exit(1);
            }
        }
        else if(!str.compare("geometry"))
        {
            bool finishedGeometry = false;
            while(!stream.eof())
            {
                stream.getline(s, 4096);
                std::stringstream l(s);
                l >> str;
                if(str[0] == '#')
                    continue;
                if(!str.compare("sphere"))
                {
                    bool finishedSphere = false;
                    Point pos;
                    float radius;
                    std::string materialId;
                    while(!stream.eof())
                    {
                        stream.getline(s, 4096);
                        std::stringstream l(s);
                        l >> str;
                        if(str[0] == '#')
                            continue;
                        if(!str.compare("coords"))
                            l >> pos;
                        else if(!str.compare("radius"))
                            l >> radius;
                        else if(!str.compare("material"))
                            l >> materialId;
                        else if(!str.compare("endsphere"))
                        {
                            finishedSphere = true;
                            objects.push_back(new Sphere(radius, pos,
                                                         materials.at(materialId), materials.at(materialId)));
                            break;
                        }
                    }
                    if(!finishedSphere)
                    {
                        std::cerr << "Invalid file format " << filepath << std::endl;
                        exit(1);
                    }
                }
                else if(!str.compare("triangle"))
                {
                    bool finishedTriangle = false;
                    int pId = 0;
                    Point p[3];
                    std::string materialId;
                    while(!stream.eof())
                    {
                        stream.getline(s, 4096);
                        std::stringstream l(s);
                        l >> str;
                        if(str[0] == '#')
                            continue;
                        if(!str.compare("vertex"))
                            l >> p[pId++];
                        else if(!str.compare("material"))
                            l >> materialId;
                        else if(!str.compare("endtriangle"))
                        {
                            if(pId < 3)
                                break;
                            finishedTriangle  = true;
                            objects.push_back(new Polygon(p[0], p[1], p[2],
                                                         materials.at(materialId), materials.at(materialId)));
                            break;
                        }
                    }
                    if(!finishedTriangle)
                    {
                        std::cerr << "Invalid file format " << filepath << std::endl;
                        exit(1);
                    }
                }
                else if(!str.compare("quadrangle"))
                {
                    bool finishedQuadrangle = false;
                    int pId = 0;
                    Point p[4];
                    std::string materialId;
                    while(!stream.eof())
                    {
                        stream.getline(s, 4096);
                        std::stringstream l(s);
                        l >> str;
                        if(str[0] == '#')
                            continue;
                        if(!str.compare("vertex"))
                            l >> p[pId++];
                        else if(!str.compare("material"))
                            l >> materialId;
                        else if(!str.compare("endquadrangle"))
                        {
                            if(pId < 4)
                                break;
                            finishedQuadrangle = true;
                            objects.push_back(new Quadrangle(p[0], p[1], p[2], p[3],
                                                         materials.at(materialId), materials.at(materialId)));
                            break;
                        }
                    }
                    if(!finishedQuadrangle)
                    {
                        std::cerr << "Invalid file format " << filepath << std::endl;
                        exit(1);
                    }
                }
                else if(!str.compare("stlmodel"))
                {
                    bool finishedModel = false;
                    std::string modelname;
                    Point move;
                    Model* model;
                    while(!stream.eof())
                    {
                        stream.getline(s, 4096);
                        std::stringstream l(s);
                        l >> str;
                        if(str[0] == '#')
                            continue;
                        if(!str.compare("filename"))
                        {
                            l >> modelname;
                            model = STLBinarySerializer().LoadModel(modelname);
                        }
                        else if(!str.compare("move"))
                            l >> move;
                        else if(!str.compare("endstlmodel"))
                        {
                            finishedModel = true;
                            model->ApplyTransformation(Transform::Move(move));
                            objects.insert(objects.end(), model->GetGeometry().begin(), model->GetGeometry().end());
                            break;
                        }
                    }
                    if(!finishedModel)
                    {
                        std::cerr << "Invalid file format " << filepath << std::endl;
                        exit(1);
                    }
                }
                else if(!str.compare("endgeometry"))
                {
                    finishedGeometry = true;
                    break;
                }
            }
            if(!finishedGeometry)
            {
                std::cerr << "Invalid file format " << filepath << std::endl;
                exit(1);
            }
        }
    }

    auto tree = new KDFairTree(19);//KDTree(10.0, 1.0);//
    tree->Initialize(objects);
    return new Scene(*camera, view, tree, lights, reference);
}
