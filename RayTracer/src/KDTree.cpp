#include "KDTree.h"

#include <algorithm>
#include <assert.h>

struct sortXMax
{
    inline bool operator() (const std::pair<Box, ISceneObject*>& obj1, const std::pair<Box, ISceneObject*>& obj2)
    {
        return (obj1.first.XMax < obj2.first.XMax);
    }
};
struct sortXMin
{
    inline bool operator() (const std::pair<Box, ISceneObject*>& obj1, const std::pair<Box, ISceneObject*>& obj2)
    {
        return (obj1.first.XMin < obj2.first.XMin);
    }
};
struct sortYMax
{
    inline bool operator() (const std::pair<Box, ISceneObject*>& obj1, const std::pair<Box, ISceneObject*>& obj2)
    {
        return (obj1.first.YMax < obj2.first.YMax);
    }
};
struct sortYMin
{
    inline bool operator() (const std::pair<Box, ISceneObject*>& obj1, const std::pair<Box, ISceneObject*>& obj2)
    {
        return (obj1.first.YMin < obj2.first.YMin);
    }
};
struct sortZMax
{
    inline bool operator() (const std::pair<Box, ISceneObject*>& obj1, const std::pair<Box, ISceneObject*>& obj2)
    {
        return (obj1.first.ZMax < obj2.first.ZMax);
    }
};
struct sortZMin
{
    inline bool operator() (const std::pair<Box, ISceneObject*>& obj1, const std::pair<Box, ISceneObject*>& obj2)
    {
        return (obj1.first.ZMin < obj2.first.ZMin);
    }
};

KDTree::KDTree(float emptySpaceCost, float maxNodeCost): primaryBox_(0, 0, 0, 0, 0, 0),
                                            emptySpaceCost_(emptySpaceCost), maxNodeCost_(maxNodeCost),
                                            root_(nullptr)
{}

void KDTree::Initialize(std::vector<ISceneObject*>& objects)
{
    std::vector<std::pair<Box, ISceneObject*>> boxes;
    for(auto obj: objects)
        boxes.push_back(std::pair<Box, ISceneObject*> (obj->GetBoundingBox(), obj) );


    if(boxes.size() > 0)
    {
        primaryBox_.XMax = xBoxes_max.back().first.XMax;
        primaryBox_.XMin = xBoxes_min[0].first.XMin;
        primaryBox_.YMax = yBoxes_max.back().first.YMax;
        primaryBox_.YMin = yBoxes_min[0].first.YMin;
        primaryBox_.ZMax = zBoxes_max.back().first.ZMax;
        primaryBox_.ZMin = zBoxes_min[0].first.ZMin;
    }

    root_ = this->DivideAndBuild(boxes, primaryBox_);
}

KDTree::KDNode*KDTree::DivideAndBuild(std::vector<std::pair<Box, ISceneObject*>>& objects, Box box)
{
    Axis maxAxis = A_X;
    auto maxLength = box.XLength();
    if(box.YLength() > maxLenght)
    {
        maxAxis = A_Y;
        maxLength = box.YLength();
    }
    if(box.ZLength() > maxLenght)
    {
        maxAxis = A_Z;
        maxLength = box.ZLength();
    }
    auto split = box.GetMid(maxAxis);
    for(auto obj: objects)
    {
        split += obj.first.GetMid(maxAxis);
    }
    split /= objects.size();
    std::vector<std::pair<Box, ISceneObject*>> leftObjects, rightObjects;
    unsigned int commonObjects = 0;
    for(auto obj: objects)
    {
        auto maxVal = GetBoxValueMaxFromAxis(obj.first, axis);
        auto minVal = GetBoxValueMinFromAxis(obj.first, axis);
        if(maxVal > split)
            rightObjects.push_back(obj);
        if(minVal < split)
            leftObjects.push_back(obj);
        if(maxVal > split && minVal < split)
            ++commonObjects;
    }
    KDNode* node = new KDNode();
    node->box = box;
    if(commonObjects > 0.5 * objects.size())
    {
        node->leaf = true;
        for(auto obj: objects)
            node->objects.push_back(obj.second);
        return node;
    }
    node->leaf = false;
    Box left = box;
    Box right = box;
    if(maxAxis == A_X)
    {
        left.XMax = split;
        right.XMin = split;
    }
    else if(maxAxis == A_Y)
    {
        left.YMax = split;
        right.YMin = split;
    }
    else
    {
        left.ZMax = split;
        right.ZMin = split;
    }
    node->left = DivideAndBuild(leftObjects, left);
    node->right = DivideAndBuild(rightObjects, right);
    node->plane = split;
    return node;
}


Color KDTree::EmitLights(CollisionData& collision, std::vector<PointLight*>& lights)
{
    Color c = collision.PixelColor.RGBtoHSV();
    c.B = 0;
    for(auto light: lights)
    {
        Photon photon(collision.CollisionPoint, light->GetPosition() - collision.CollisionPoint, collision.Owner);
        if(this->CollideNode(this->rootX_, this->rootY_, this->rootZ_, this->primaryBox_, photon)->IsCollide)
            continue;
        Color lColor = light->GetLight();
        lColor.B *= (200.0f / std::pow((light->GetPosition() - collision.CollisionPoint).Length(),2))*
                (collision.CollisionNormal.Normalized()*photon.Direction().Normalized());
        c = c + lColor;
    }
    auto fin = c.HSVtoRGB();
    return fin;
}

Color KDTree::RenderPhoton(Photon photon, std::vector<PointLight*>& lights)
{
    Box box = this->primaryBox_;
    auto xNode = rootX_;
    auto yNode = rootY_;
    auto zNode = rootZ_;
    std::pair<Point, Point> intersects;
    if(!photon.IntersecWithBox(box, intersects))
        return Color(0, 0, 0);
    auto minCollision = this->CollideNode(xNode, yNode, zNode, box, photon);
    Color color;
    if(minCollision->IsCollide)
        color = this->EmitLights(*minCollision, lights);
    else
        color = minCollision->PixelColor;
    delete minCollision;
    return color;
}

float KDTree::GetBoxValueMaxFromAxis(const Box& box, KDTree::Axis axis) const
{
    switch(axis)
    {
        case Axis::A_X:
            return box.XMax;
        case Axis::A_Y:
            return box.YMax;
        case Axis::A_Z:
            return box.ZMax;
        default:
            break;
    }
}
float KDTree::GetBoxValueMinFromAxis(const Box& box, KDTree::Axis axis) const
{
    switch(axis)
    {
        case Axis::A_X:
            return box.XMin;
        case Axis::A_Y:
            return box.YMin;
        case Axis::A_Z:
            return box.ZMin;
        default:
            break;
    }
}

CollisionData*KDTree::CollideNode(KDTree::KDNode* node, const Photon& photon)
{
    std::pair<Point, Point> intersets;
    if(!photon.IntersecWithBox(node->box, intersets))
    {
        return new CollisionData(false);
    }
    if(!node->leaf)
    {
        auto collLeft = this->CollideNode(node->left, photon);
        auto collRight = this->CollideNode(node->left, photon);
        if(collLeft->IsCollide && collRight->IsCollide)
        {
            if((collLeft->CollisionPoint - photon.position_).Length() <
                (collRight->CollisionPoint - photon.position_).Length())
            {
                delete collRight;
                return collLeft;
            }
            delete collLeft;
            return collRight;
        }
        if(collLeft->IsCollide)
            return collLeft;
        return collRight;
    }
    auto minCollision = new CollisionData(false);
    CollisionData collision;
    for(auto obj: node->objects)
    {
        if(obj->GetCollision(collision) && obj != photon.Owner())
        {
            if(!minCollision->IsCollide || (collision.CollisionPoint - photon.Position()).Length() <
                    (minCollision->CollisionPoint - photon.Position()).Length())
            {
                *minCollision = collision;
            }
        }
    }
    return minCollision;
}
