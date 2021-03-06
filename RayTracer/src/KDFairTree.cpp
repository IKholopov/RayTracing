#include "KDFairTree.h"

#include <algorithm>
#include <future>

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


KDFairTree::KDFairTree(int depth):depth_(depth), root_(nullptr)
{
}

KDFairTree::~KDFairTree()
{
    if(root_)
        delete this->root_;
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
    SortedArrays arrays(objects);
    arrays.Sort();
    root_ = DivideAndBuild(arrays, primaryBox, 1, Axis::A_X);
}

CollisionData* KDFairTree::CheckCollide(const Photon& photon)
{
    return CheckCollideNode(this->root_, photon);
}

KDFairTree::KDFairNode* KDFairTree::DivideAndBuild(SortedArrays& objects, Box box, int depth, Axis axis, size_t launchedThreads)
{
    auto node = new KDFairNode();
    node->box = box;
    node->size = objects.GetSortedMax(Axis::A_X).size();
    if(objects.GetSortedMax(Axis::A_X).size() < 10)
    {
        node->leaf = true;
        for(auto obj: objects.GetSortedMax(Axis::A_X))
            node->objects.push_back(obj);
        return node;
    }
    float optimalSplit ;
    if(depth > 9)
    {
        optimalSplit = objects.XMax[0]->GetBoundingBox().GetMid(axis) / (float)(objects.XMax.size());
        for(auto obj: objects.XMax)
        {
            optimalSplit += obj->GetBoundingBox().GetMid(axis) / (float)(objects.XMax.size());
        }
    }
    else {
        std::vector<ISceneObject*>& minSorted = objects.GetSortedMin(axis);
        std::vector<ISceneObject*>& maxSorted = objects.GetSortedMax(axis);
        long leftObjects = 0;
        long rightObjects = maxSorted.size();
        optimalSplit = maxSorted[0]->GetBoundingBox().GetMax(axis);
        float sah = std::numeric_limits<float>::max();
            auto minBorder = minSorted.begin();
            for(auto obj = maxSorted.begin(); obj != maxSorted.end() - 1; ++obj)
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
            rightObjects = objects.XMax.size();
            minBorder = maxSorted.begin();
            for(auto obj = minSorted.begin() + 1; obj != minSorted.end(); ++obj)
            {
                leftObjects++;
                float split = (*obj)->GetBoundingBox().GetMin(axis) - 0.0001;
                while(minBorder != maxSorted.end() && (*minBorder)->GetBoundingBox().GetMax(axis) < split)
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
    SortedArrays left, right;
    objects.Split(axis, optimalSplit,left, right);
    if(depth > 2*this->depth_ || (depth > this->depth_  && (left.XMax.size() == objects.XMax.size() || right.XMax.size() == objects.XMax.size())))
    {
        node->leaf = true;
        for(auto obj: objects.XMax)
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

    if(left.XMax.size() > 10000 && right.XMax.size() > 10000 &&
            std::thread::hardware_concurrency() >= 2*launchedThreads)
    {
        auto handleLeft = std::async(std::launch::async, &KDFairTree::DivideAndBuild, this, std::ref(left), leftBox, depth+1, (Axis)((axis+1)%3), 2*launchedThreads);
        auto handleRight = std::async(std::launch::async, &KDFairTree::DivideAndBuild, this, std::ref(right), rightBox, depth+1, (Axis)((axis+1)%3),2*launchedThreads);
        node->left = handleLeft.get();
        node->right = handleRight.get();
    }
    else
    {
        node->left = DivideAndBuild(left, leftBox, depth+1, (Axis)((axis+1)%3), launchedThreads);
        node->right = DivideAndBuild(right, rightBox, depth+1, (Axis)((axis+1)%3), launchedThreads);
    }

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
        if(obj->GetCollision(photon, data) && (photon.Owner() != obj  || (!obj->GetType().compare("sphere")) &&
                                               !IsFloatZero((photon.Position() - data.CollisionPoint).Length())) &&
                node->box.IsInside(data.CollisionPoint) &&
                (!minCollision->IsCollide || (data.CollisionPoint - photon.Position()).Length() <
                 (minCollision->CollisionPoint - photon.Position()).Length()))
            *minCollision = data;
    }
    return minCollision;
}
CollisionData*KDFairTree::CheckCollideNode(KDFairTree::KDFairNode* node, const Photon& photon)
{
    if(!photon.IntersecWithBox(node->box))
        return new CollisionData(false);
    if(!node->leaf)
    {
        Box b1 = node->left->box;
        Box b2 = node->right->box;
        if(!photon.IntersecWithBox(b1))
            return CheckCollideNode(node->right, photon);
        if(!photon.IntersecWithBox(b2))
            return CheckCollideNode(node->left, photon);
        if(photon.Position().GetAxis(node->axis) <= node->plane)
        {
            auto collData = CheckCollideNode(node->left, photon);
            if(collData->IsCollide)
                return collData;
            delete collData;
            return CheckCollideNode(node->right, photon);
        }
        else
        {
            auto collData = CheckCollideNode(node->right, photon);
            if(collData->IsCollide)
                return collData;
            delete collData;
            return CheckCollideNode(node->left, photon);
        }
    }
    auto minCollision = new CollisionData(false);
    for(auto obj: node->objects)
    {
        CollisionData data(false);
        if(obj->GetCollision(photon, data) && photon.Owner() != obj &&
                node->box.IsInside(data.CollisionPoint))
        {
            *minCollision = data;
            break;
        }
    }
    return minCollision;
}

KDFairTree::SortedArrays::SortedArrays()
{

}

KDFairTree::SortedArrays::SortedArrays(std::vector<ISceneObject*>& objects)
{
    this->XMax = objects;
    this->XMin = objects;
    this->YMax = objects;
    this->YMin = objects;
    this->ZMax = objects;
    this->ZMin = objects;
}

void KDFairTree::SortedArrays::Sort()
{
    std::sort(XMax.begin(), XMax.end(), sortXMax());
    std::sort(XMin.begin(), XMin.end(), sortXMin());
    std::sort(YMax.begin(), YMax.end(), sortYMax());
    std::sort(YMin.begin(), YMin.end(), sortYMin());
    std::sort(ZMax.begin(), ZMax.end(), sortZMax());
    std::sort(ZMin.begin(), ZMin.end(), sortZMin());
}

void KDFairTree::SortedArrays::Split(Axis axis, float plane, SortedArrays& array1, SortedArrays& array2)
{

        /*array1.XMax.reserve(this->XMax.size());
        array1.YMax.reserve(this->XMax.size());
        array1.ZMax.reserve(this->XMax.size());
        array1.XMin.reserve(this->XMax.size());
        array1.YMin.reserve(this->XMax.size());
        array1.ZMin.reserve(this->XMax.size());
        array2.XMax.reserve(this->XMax.size());
        array2.YMax.reserve(this->XMax.size());
        array2.ZMax.reserve(this->XMax.size());
        array2.XMin.reserve(this->XMax.size());
        array2.YMin.reserve(this->XMax.size());
        array2.ZMin.reserve(this->XMax.size());*/
        for(auto obj: XMax)
        {
            if(obj->GetBoundingBox().GetMin(axis) <= plane)
                array1.XMax.push_back(obj);
            if(obj->GetBoundingBox().GetMax(axis) >= plane)
                array2.XMax.push_back(obj);
        }
        for(auto obj: XMin)
        {
            if(obj->GetBoundingBox().GetMin(axis) <= plane)
                array1.XMin.push_back(obj);
            if(obj->GetBoundingBox().GetMax(axis) >= plane)
                array2.XMin.push_back(obj);
        }
        for(auto obj: YMax)
        {
            if(obj->GetBoundingBox().GetMin(axis) <= plane)
                array1.YMax.push_back(obj);
            if(obj->GetBoundingBox().GetMax(axis) >= plane)
                array2.YMax.push_back(obj);
        }
        for(auto obj: YMin)
        {
            if(obj->GetBoundingBox().GetMin(axis) <= plane)
                array1.YMin.push_back(obj);
            if(obj->GetBoundingBox().GetMax(axis) >= plane)
                array2.YMin.push_back(obj);
        }
        for(auto obj: ZMax)
        {
            if(obj->GetBoundingBox().GetMin(axis) <= plane)
                array1.ZMax.push_back(obj);
            if(obj->GetBoundingBox().GetMax(axis) >= plane)
                array2.ZMax.push_back(obj);
        }
        for(auto obj: ZMin)
        {
            if(obj->GetBoundingBox().GetMin(axis) <= plane)
                array1.ZMin.push_back(obj);
            if(obj->GetBoundingBox().GetMax(axis) >= plane)
                array2.ZMin.push_back(obj);
        }
}

std::vector<ISceneObject*>& KDFairTree::SortedArrays::GetSortedMax(Axis axis)
{
    if(axis == Axis::A_X)
        return this->XMax;
    if(axis == Axis::A_Y)
        return this->YMax;
    else
        return this->ZMax;
}

std::vector<ISceneObject*>& KDFairTree::SortedArrays::GetSortedMin(Axis axis)
{
    if(axis == Axis::A_X)
        return this->XMin;
    if(axis == Axis::A_Y)
        return this->YMin;
    else
        return this->ZMin;
}

KDFairTree::KDFairNode::~KDFairNode()
{
    if(!this->leaf)
    {
        delete this->left;
        delete this->right;
    }
}
