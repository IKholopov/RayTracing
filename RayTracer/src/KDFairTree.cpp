#include "KDFairTree.h"

#include <algorithm>

struct sortXMax
{
    inline bool operator() (ISceneObject*& obj1, ISceneObject*& obj2)
    {
        return (obj1->GetBoundingBox().XMax < obj2->GetBoundingBox().XMax);
    }
};
struct sortXMin
{
    inline bool operator() (ISceneObject*& obj1, ISceneObject*& obj2)
    {
        return (obj1->GetBoundingBox().XMin < obj2->GetBoundingBox().XMin);
    }
};
struct sortYMax
{
    inline bool operator() (ISceneObject*& obj1, ISceneObject*& obj2)
    {
        return (obj1->GetBoundingBox().YMax < obj2->GetBoundingBox().YMax);
    }
};
struct sortYMin
{
    inline bool operator() (ISceneObject*& obj1, ISceneObject*& obj2)
    {
        return (obj1->GetBoundingBox().YMin < obj2->GetBoundingBox().YMin);
    }
};
struct sortZMax
{
    inline bool operator() (ISceneObject*& obj1, ISceneObject*& obj2)
    {
        return (obj1->GetBoundingBox().ZMax < obj2->GetBoundingBox().ZMax);
    }
};
struct sortZMin
{
    inline bool operator() (ISceneObject*& obj1, ISceneObject*& obj2)
    {
        return (obj1->GetBoundingBox().ZMin < obj2->GetBoundingBox().ZMin);
    }
};


KDFairTree::KDFairTree(int depth):depth_(depth)
{
}

void KDFairTree::Initialize(std::vector<ISceneObject*>& objects)
{
    if(objects.size() == 0)
    {
        root_ = nullptr;
        return;
    }
    Box primaryBox = objects.at(0)->GetBoundingBox();
    for(auto obj: objects)
    {
        auto box = obj->GetBoundingBox();
        if(box.XMax > primaryBox.XMax)
            primaryBox.XMax = box.XMax;
        if(box.XMin < primaryBox.XMin)
            primaryBox.XMin = box.XMin;
        if(box.YMax > primaryBox.YMax)
            primaryBox.YMax = box.YMax;
        if(box.YMin < primaryBox.YMin)
            primaryBox.YMin = box.YMin;
        if(box.ZMax > primaryBox.ZMax)
            primaryBox.ZMax = box.ZMax;
        if(box.ZMin < primaryBox.ZMin)
            primaryBox.ZMin = box.ZMin;
    }
    this->primaryBox_ = primaryBox;
    root_ = DivideAndBuild(objects, primaryBox, 1);
}

KDFairTree::KDFairNode* KDFairTree::DivideAndBuild(std::vector<ISceneObject*>& objects, Box box, int depth)
{
    auto minAxis = Axis::A_X;
    if(box.YLength() > box.XLength())
        minAxis = A_Y;
    if(box.ZLength() > box.YLength() && box.ZLength() > box.XLength())
        minAxis = A_Z;
    if(minAxis == A_X)
    {
        std::sort(objects.begin(), objects.end(), sortXMax());
    }
    else if(minAxis == A_Y)
    {
        std::sort(objects.begin(), objects.end(), sortYMax());
    }
    else
    {
        std::sort(objects.begin(), objects.end(), sortZMax());
    }
    /*
    long leftObjects = 0;
    long rightObjects = objects.size();
    float optimalSplit = objects[0]->GetBoundingBox().GetMax(minAxis);
    float sah = leftObjects*(objects[0]->GetBoundingBox().GetMax(minAxis) - box.GetMin(minAxis)) +
                rightObjects*(box.GetMax(minAxis) - objects[0]->GetBoundingBox().GetMax(minAxis));
    for(auto obj = objects.begin(); obj != objects.end() - 1; ++obj)
    {
        ++leftObjects;
        --rightObjects;
        float split = (*obj)->GetBoundingBox().GetMax(minAxis);
        auto surfLeft = ((*obj)->GetBoundingBox().GetMax(minAxis) - box.GetMin(minAxis));
        auto surfRight = (box.GetMax(minAxis) - (*obj)->GetBoundingBox().GetMax(minAxis));
        float newSah = leftObjects*surfLeft +
                rightObjects*surfRight;
        if(newSah < sah){
            optimalSplit = split;
            sah = newSah;
        }
    }
    if(minAxis == A_X)
    {
        std::sort(objects.begin(), objects.end(), sortXMin());
    }
    else if(minAxis == A_Y)
    {
        std::sort(objects.begin(), objects.end(), sortYMin());
    }
    else
    {
        std::sort(objects.begin(), objects.end(), sortZMin());
    }
    leftObjects = -1;
    rightObjects = objects.size();
    for(auto obj = objects.begin(); obj != objects.end() - 1; ++obj)
    {
        ++leftObjects;
        --rightObjects;
        float split = (*obj)->GetBoundingBox().GetMin(minAxis);
        float newSah = leftObjects*((*obj)->GetBoundingBox().GetMin(minAxis) - box.GetMin(minAxis)) +
                rightObjects*(box.GetMax(minAxis) - (*obj)->GetBoundingBox().GetMin(minAxis));
        if(newSah < sah){
            optimalSplit = split;
            sah = newSah;
        }
    }*/
    std::vector<ISceneObject*> left, right;
    float optimalSplit = box.GetMid(minAxis);
    for(auto obj: objects)
    {
        if(obj->GetBoundingBox().GetMin(minAxis) <= optimalSplit)
            left.push_back(obj);
        if(obj->GetBoundingBox().GetMax(minAxis) >= optimalSplit)
            right.push_back(obj);
    }
    auto node = new KDFairNode();
    node->box = box;
    node->size = objects.size();
    if(depth > this->depth_ )//|| left.size() > 0.9 * objects.size() && right.size() > 0.9 * objects.size() )
    {
        node->leaf = true;
        for(auto obj: objects)
            node->objects.push_back(obj);
        return node;
    }
    node->leaf = false;
    node->plane = optimalSplit;
    node->axis = minAxis;
    Box leftBox = box;
    leftBox.SetAxisMax(minAxis, optimalSplit);
    Box rightBox = box;
    rightBox.SetAxisMin(minAxis, optimalSplit);
    node->left = DivideAndBuild(left, leftBox, depth+1);
    node->right = DivideAndBuild(right, rightBox, depth+1);

    return node;
}

CollisionData*KDFairTree::RenderPhoton(Photon photon)
{
    return CollideNode(this->root_, photon);
}

CollisionData*KDFairTree::CollideNode(KDFairTree::KDFairNode* node, const Photon& photon)
{
if(!photon.IntersecWithBox(node->box))
        return new CollisionData(false);
    if(!node->leaf)
    {
        Box b1 = node->left->box;
        Box b2 = node->right->box;
        if(!photon.IntersecWithBox(b1))
            return CollideNode(node->right, photon);
        if(!photon.IntersecWithBox(b2))
            return CollideNode(node->left, photon);
        if(photon.Position().GetAxis(node->axis) <= node->plane)
        {
            auto collData = CollideNode(node->left, photon);
            if(collData->IsCollide)
                return collData;
            delete collData;
            return CollideNode(node->right, photon);
        }
        else
        {
            auto collData = CollideNode(node->right, photon);
            if(collData->IsCollide)
                return collData;
            delete collData;
            return CollideNode(node->left, photon);
        }
    }
    auto minCollision = new CollisionData(false);
    for(auto obj: node->objects)
    {
        CollisionData data(false);
        if(obj->GetCollision(photon, data) && photon.Owner() != obj &&
                node->box.IsInside(data.CollisionPoint) &&
                (!minCollision->IsCollide || (data.CollisionPoint - photon.Position()).Length() <
                 (minCollision->CollisionPoint - photon.Position()).Length()))
            *minCollision = data;
    }
    return minCollision;
}
