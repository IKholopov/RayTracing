#include "KDTree.h"

#include <algorithm>
#include <assert.h>
#include <limits>

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
                                            root_(nullptr), size_(0)
{}

void KDTree::Initialize(std::vector<ISceneObject*>& objects)
{
    this->size_ = objects.size();
    std::vector<std::pair<Box, ISceneObject*>> xBoxes_max;
    for(auto obj: objects)
        xBoxes_max.push_back(std::pair<Box, ISceneObject*> (obj->GetBoundingBox(), obj) );
    std::sort(xBoxes_max.begin(), xBoxes_max.end(), sortXMax());
    std::vector<std::pair<Box, ISceneObject*>> xBoxes_min(xBoxes_max);
    std::sort(xBoxes_min.begin(), xBoxes_min.end(), sortXMin());
    std::vector<std::pair<Box, ISceneObject*>> yBoxes_max(xBoxes_max);
    std::sort(yBoxes_max.begin(), yBoxes_max.end(), sortYMax());
    std::vector<std::pair<Box, ISceneObject*>> yBoxes_min(xBoxes_max);
    std::sort(yBoxes_min.begin(), yBoxes_min.end(), sortYMin());
    std::vector<std::pair<Box, ISceneObject*>> zBoxes_max(xBoxes_max);
    std::sort(zBoxes_max.begin(), zBoxes_max.end(), sortZMax());
    std::vector<std::pair<Box, ISceneObject*>> zBoxes_min(xBoxes_max);
    std::sort(zBoxes_min.begin(), zBoxes_min.end(), sortZMin());


    if(xBoxes_max.size() > 0)
    {
        primaryBox_.XMax = xBoxes_max.back().first.XMax;
        primaryBox_.XMin = xBoxes_min[0].first.XMin;
        primaryBox_.YMax = yBoxes_max.back().first.YMax;
        primaryBox_.YMin = yBoxes_min[0].first.YMin;
        primaryBox_.ZMax = zBoxes_max.back().first.ZMax;
        primaryBox_.ZMin = zBoxes_min[0].first.ZMin;
    }

    root_ = this->DivideAndBuild(xBoxes_max, primaryBox_);
}

CollisionData*KDTree::CollidePhoton(const Photon& photon)
{
    return CollideNode(this->root_, photon);
}

unsigned int KDTree::Size()
{
    return size_;
}

KDTree::KDNode*KDTree::DivideAndBuild(std::vector<std::pair<Box, ISceneObject*> >& objects, Box box)
{
    Axis maxAxis = A_X;
    auto maxLength = box.XLength();
    if(box.YLength() > maxLength)
    {
        maxAxis = A_Y;
        maxLength = box.YLength();
    }
    if(box.ZLength() > maxLength)
    {
        maxAxis = A_Z;
        maxLength = box.ZLength();
    }
    auto split = box.GetMid(maxAxis);
    for(auto obj : objects)
    {
        split += obj.first.GetMid(maxAxis);
    }
    split /= objects.size();
    std::vector<std::pair<Box, ISceneObject*>> leftObjects, rightObjects;
    unsigned int commonObjects = 0;
    for(auto obj: objects)
    {
        auto maxVal = GetBoxValueMaxFromAxis(obj.first, maxAxis);
        auto minVal = GetBoxValueMinFromAxis(obj.first, maxAxis);
        if(maxVal > split)
            rightObjects.push_back(obj);
        if(minVal < split)
            leftObjects.push_back(obj);
        if(maxVal > split && minVal < split)
            ++commonObjects;
    }
    KDNode* node = new KDNode();
    node->box = box;
    if(commonObjects > 0.5 * objects.size() || leftObjects.size() == objects.size() ||
            rightObjects.size() == objects.size())
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
    node->axis = maxAxis;
    return node;
}


Color KDTree::EmitLights(CollisionData& collision, std::vector<PointLight*>& lights)
{
    Color c = collision.PixelColor.RGBtoHSV();
    c.B = 0;
    for(auto light: lights)
    {
        Photon photon(collision.CollisionPoint, light->GetPosition() - collision.CollisionPoint, collision.Owner);
        if(this->CollideNode(root_, photon)->IsCollide)
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
    std::pair<Point, Point> intersects;
    if(!photon.IntersecWithBox(box, intersects))
        return Color(0, 0, 0);
    auto minCollision = this->CollideNode(root_, photon);
    Color color;
    if(minCollision->IsCollide)
        color = this->EmitLights(*minCollision, lights);
    else
        color = minCollision->PixelColor;
    delete minCollision;
    return color;
}

float KDTree::GetBoxValueMaxFromAxis(const Box& box, Axis axis) const
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
float KDTree::GetBoxValueMinFromAxis(const Box& box, Axis axis) const
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
        Box b1 = node->left->box;
        Box b2 = node->right->box;
        std::pair<Point, Point> b1Intersects;
        std::pair<Point, Point> b2Intersects;

        if(b1.IsInside(photon.Position()) || !photon.IntersecWithBox(b2, b2Intersects) ||
                (photon.IntersecWithBox(b1, b1Intersects) &&
                 (b1Intersects.first - photon.Position()).Length()  <= (b2Intersects.first - photon.Position()).Length()))
        {
            auto collLeft = this->CollideNode(node->left, photon);
            if(collLeft->IsCollide && collLeft->CollisionPoint.GetAxis(node->axis) < node->plane)
                return collLeft;
            delete collLeft;
            return this->CollideNode(node->right, photon);
        }
        else
        {
            auto collRight = this->CollideNode(node->right, photon);
            if(collRight->IsCollide && collRight->CollisionPoint.GetAxis(node->axis) > node->plane)
                return collRight;
            delete collRight;
            return this->CollideNode(node->left, photon);
        }
    }
    auto minCollision = new CollisionData(false);
    CollisionData collision(false);
    for(auto obj: node->objects)
    {
        if(obj->GetCollision(photon, collision) && obj != photon.Owner())
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
