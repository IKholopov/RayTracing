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
    root_ = DivideAndBuild(objects, primaryBox, 1, Axis::A_X, 1);
}

KDFairTree::KDFairNode* KDFairTree::DivideAndBuild(std::vector<ISceneObject*>& objects, Box box, int depth, Axis axis, bool divideByMax)
{
    auto node = new KDFairNode();
    node->box = box;
    node->size = objects.size();
    if(objects.size() < 4)
    {
        node->leaf = true;
        for(auto obj: objects)
            node->objects.push_back(obj);
        return node;
    }
    float optimalSplit ;
    if(depth > 10)
        optimalSplit = box.GetMid(axis);
    else {
        std::vector<ISceneObject*>& minSorted(objects);
        if(axis == A_X)
        {
            std::sort(objects.begin(), objects.end(), sortXMax());
            std::sort(minSorted.begin(), minSorted.end(), sortXMin());
        }
        else if(axis == A_Y)
        {
            std::sort(objects.begin(), objects.end(), sortYMax());
            std::sort(minSorted.begin(), minSorted.end(), sortYMin());
        }
        else
        {
            std::sort(objects.begin(), objects.end(), sortZMax());
            std::sort(minSorted.begin(), minSorted.end(), sortZMin());
        }
        long leftObjects = 0;
        long rightObjects = objects.size();
        optimalSplit = objects[0]->GetBoundingBox().GetMax(axis);
        float sah = std::numeric_limits<float>::max();
        if(divideByMax)
        {
            auto minBorder = minSorted.begin();
            for(auto obj = objects.begin(); obj != objects.end() - 1; ++obj)
            {
                float split = (*obj)->GetBoundingBox().GetMax(axis) + 0.0001;
                while(minBorder != minSorted.end() && (*minBorder)->GetBoundingBox().GetMin(axis) < split)
                {
                    ++minBorder;
                    leftObjects++;
                }
                rightObjects--;
                auto surfLeft = ((*obj)->GetBoundingBox().GetMax(axis) - box.GetMin(axis));
                auto surfRight = (box.GetMax(axis) - (*obj)->GetBoundingBox().GetMax(axis));
                float newSah = leftObjects*surfLeft +
                        rightObjects*surfRight;
                if(newSah < sah){
                    optimalSplit = split;
                    sah = newSah;
                }
            }
        }
        else
        {
            auto minBorder = objects.begin();
            for(auto obj = minSorted.begin(); obj != minSorted.end() - 1; ++obj)
            {
                leftObjects++;
                float split = (*obj)->GetBoundingBox().GetMin(axis) - 0.0001;
                while(minBorder != objects.end() && (*minBorder)->GetBoundingBox().GetMax(axis) < split)
                {
                    ++minBorder;
                    rightObjects--;
                }
                float newSah = leftObjects*((*obj)->GetBoundingBox().GetMin(axis) - box.GetMin(axis)) +
                        rightObjects*(box.GetMax(axis) - (*obj)->GetBoundingBox().GetMin(axis));
                if(newSah < sah){
                    optimalSplit = split;
                    sah = newSah;
                }
            }
        }
    }
    std::vector<ISceneObject*> left, right;
    for(auto obj: objects)
    {
        if(obj->GetBoundingBox().GetMin(axis) <= optimalSplit)
            left.push_back(obj);
        if(obj->GetBoundingBox().GetMax(axis) >= optimalSplit)
            right.push_back(obj);
    }
    if(depth > this->depth_)//|| left.size() == 0 || right.size() == 0 ||
        //left.size() == objects.size() || right.size() == objects.size())
    {
        node->leaf = true;
        for(auto obj: objects)
            node->objects.push_back(obj);
        return node;
    }
    node->leaf = false;
    node->plane = optimalSplit;
    node->axis = axis;
    Box leftBox = box;
    leftBox.SetAxisMax(axis, optimalSplit);
    Box rightBox = box;
    rightBox.SetAxisMin(axis, optimalSplit);

    node->left = DivideAndBuild(left, leftBox, depth+1, (Axis)((axis+1)%3), !divideByMax);
    node->right = DivideAndBuild(right, rightBox, depth+1, (Axis)((axis+1)%3), !divideByMax);

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
