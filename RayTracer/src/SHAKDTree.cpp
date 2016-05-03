#include "SHAKDTree.h"

/*#include <algorithm>
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

SHAKDTree::SHAKDTree(float emptySpaceCost, float maxNodeCost, unsigned int maxDepth): primaryBox_(0, 0, 0, 0, 0, 0),
                                            emptySpaceCost_(emptySpaceCost), maxNodeCost_(maxNodeCost),
                                            rootX_(nullptr), rootY_(nullptr), rootZ_(nullptr), maxDepth_(maxDepth)
{}

void SHAKDTree::Initialize(std::vector<ISceneObject*>& objects)
{
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

    rootX_ = DivideAndBuild(Axis::A_X, xBoxes_max.begin(), xBoxes_max.end()-1, xBoxes_min.begin(), xBoxes_min.end()-1,
                            primaryBox_.XMin - 1, primaryBox_.XMax + 1, 0);
    rootY_ = DivideAndBuild(Axis::A_Y, yBoxes_max.begin(), yBoxes_max.end()-1, yBoxes_min.begin(), yBoxes_min.end()-1,
                            primaryBox_.YMin - 1, primaryBox_.YMax + 1, 0);
    rootZ_ = DivideAndBuild(Axis::A_Z, zBoxes_max.begin(), zBoxes_max.end()-1, zBoxes_min.begin(), zBoxes_min.end()-1,
                            primaryBox_.ZMin - 1, primaryBox_.ZMax + 1, 0);
}

SHAKDTree::SHAKDNode*SHAKDTree::DivideAndBuild(SHAKDTree::Axis axis, std::vector<std::pair<Box, ISceneObject*>>::const_iterator maxLeft,
                                      std::vector<std::pair<Box, ISceneObject*>>::const_iterator maxRight,
                                      std::vector<std::pair<Box, ISceneObject*>>::const_iterator minLeft,
                                      std::vector<std::pair<Box, ISceneObject*>>::const_iterator minRight,
                                      float min, float max, unsigned int depth)
{
    float lMax = GetBoxValueMaxFromAxis(maxLeft->first, axis);
    float rMax = GetBoxValueMaxFromAxis(maxRight->first, axis);
    if(depth > this->maxDepth_  || maxNodeCost_ > emptySpaceCost_ + (rMax - lMax)*(std::distance(maxLeft, maxRight) + 1))
    {
        auto node = new SHAKDNode(axis);
        std::vector<ISceneObject*> objects;
        node->leaf = true;
        for(auto it = maxLeft; it != maxRight + 1; ++it)
            objects.push_back(it->second);
        node->tree = new KDTree(emptySpaceCost_, maxNodeCost_);
        node->tree->Initialize(objects);
        return node;
    }
    if(maxLeft == maxRight)
    {
        auto node = new SHAKDNode(axis);
        node->leaf = true;
        node->tree = new KDTree(emptySpaceCost_, maxNodeCost_);
        std::vector<ISceneObject*> objects;
        objects.push_back(maxLeft->second);
        node->tree->Initialize(objects);
        return node;
    }
    float optimalSplit = GetBoxValueMaxFromAxis(maxLeft->first, axis);//((-31)*GetBoxValueMaxFromAxis((maxLeft+1)->first, axis) + 95*GetBoxValueMaxFromAxis(maxLeft->first, axis)) / 64;
    unsigned int leftObjects = 0;
    unsigned int rightObjects = std::distance(maxLeft, maxRight) + 1 - leftObjects;
    assert(rightObjects > 0);
    unsigned int optimalLeftMaxObjects = leftObjects;
    unsigned int optimalRightMaxObjects = rightObjects;
    float optimalSAH = emptySpaceCost_ + leftObjects * (optimalSplit - lMax) + rightObjects * (rMax - optimalSplit) + 1;
    bool foundOptimalSAH = false;
    for(auto it = maxLeft; it != maxRight; ++it)
    {
        leftObjects++;
        rightObjects--;

        assert(rightObjects > 0);
        float split = GetBoxValueMaxFromAxis(it->first, axis);//((-31)*GetBoxValueMaxFromAxis((it+1)->first, axis) + 95*GetBoxValueMaxFromAxis(it->first, axis))/64 ;
        float sah = emptySpaceCost_ + leftObjects * (split - lMax) + rightObjects * (rMax - split);
        if(GetBoxValueMinFromAxis((it+1)->first, axis) <= min || GetBoxValueMaxFromAxis((it+1)->first, axis) >= max )
            continue;
        if(sah < optimalSAH)
        {
            foundOptimalSAH = true;
            optimalSAH = sah;
            optimalLeftMaxObjects = leftObjects;
            optimalRightMaxObjects = rightObjects;
            optimalSplit = split;
        }
    }
    float lMin = GetBoxValueMinFromAxis(minLeft->first, axis);
    float rMin = GetBoxValueMinFromAxis(minRight->first, axis);
    rightObjects = 0;
    bool foundOptimalInMin = false;
    leftObjects = std::distance(minLeft, minRight) + 1 - rightObjects;
    unsigned int optimalLeftMinObjects = leftObjects;
    unsigned int optimalRightMinObjects = rightObjects;
    for(auto it = minRight; it != minLeft; --it)
    {
        leftObjects--;
        rightObjects++;
        assert(leftObjects > 0);
        if(GetBoxValueMinFromAxis((it-1)->first, axis) <= min || GetBoxValueMaxFromAxis((it-1)->first, axis) >= max )
            continue;
        float split = GetBoxValueMinFromAxis(it->first, axis);//(95*GetBoxValueMinFromAxis((it-1)->first, axis) + (-31)*GetBoxValueMinFromAxis(it->first, axis)) / 64;
        float sah = emptySpaceCost_ + leftObjects * (split - lMin) + rightObjects * (rMin - split);
        if(sah < optimalSAH)
        {
            foundOptimalInMin = true;
            foundOptimalSAH = true;
            optimalSAH = sah;
            optimalLeftMinObjects = leftObjects;
            optimalRightMinObjects = rightObjects;
            optimalSplit = split;
        }
    }
    if(!foundOptimalSAH)
    {
        auto node = new SHAKDNode(axis);
        node->leaf = true;
        std::vector<ISceneObject*> objects;
        for(auto it = maxLeft; it != maxRight + 1; ++it)
            objects.push_back(it->second);
        node->tree = new KDTree(emptySpaceCost_, maxNodeCost_);
        node->tree->Initialize(objects);
        return node;
    }
        auto node = new SHAKDNode(axis);
        node->leaf = false;
        std::vector<std::pair<Box, ISceneObject*>> leftMax;
        std::vector<std::pair<Box, ISceneObject*>> leftMin;
        std::vector<std::pair<Box, ISceneObject*>> rightMax;
        std::vector<std::pair<Box, ISceneObject*>> rightMin;
        for(auto it = minLeft; it != minRight + 1; ++it)
        {
            if(IsFloatZero(GetBoxValueMaxFromAxis(it->first, axis) - optimalSplit) ||
                IsFloatZero(GetBoxValueMinFromAxis(it->first, axis) - optimalSplit))
                {
                    //if(IsFloatZero(GetBoxValueMaxFromAxis(it->first, axis) - optimalSplit) &&
                      //      GetBoxValueMinFromAxis(it->first, axis) <= optimalSplit)
                        leftMin.push_back(*(it));
                    //if(IsFloatZero(GetBoxValueMinFromAxis(it->first, axis) - optimalSplit) &&
                      //      GetBoxValueMaxFromAxis(it->first, axis) >= optimalSplit)
                        rightMin.push_back(*(it));
                }
                else
                {
                    if(GetBoxValueMaxFromAxis(it->first, axis) > optimalSplit)
                        rightMin.push_back(*(it));
                    if(GetBoxValueMinFromAxis(it->first, axis) < optimalSplit)
                        leftMin.push_back(*(it));
                }
        }
        for(auto it = maxLeft; it != maxRight + 1; ++it)
            {
                if(IsFloatZero(GetBoxValueMaxFromAxis(it->first, axis) - optimalSplit) ||
                        IsFloatZero(GetBoxValueMinFromAxis(it->first, axis) - optimalSplit))
                {
                    //if(IsFloatZero(GetBoxValueMaxFromAxis(it->first, axis) - optimalSplit) &&
                      //      GetBoxValueMinFromAxis(it->first, axis) <= optimalSplit)
                        leftMax.push_back(*(it));
                    //if(IsFloatZero(GetBoxValueMinFromAxis(it->first, axis) - optimalSplit) &&
                      //      GetBoxValueMaxFromAxis(it->first, axis) >= optimalSplit)
                        rightMax.push_back(*(it));
                }
                else
                {
                    if(GetBoxValueMinFromAxis(it->first, axis) < optimalSplit)
                        leftMax.push_back(*(it));
                    if(GetBoxValueMaxFromAxis(it->first, axis) > optimalSplit)
                        rightMax.push_back(*(it));
                }
            }
        if(leftMin.size() == 0)
        {
            node->leaf = true;
            std::vector<ISceneObject*> objects;
            for(auto it = rightMin.begin(); it != rightMin.end(); ++it)
                objects.push_back(it->second);
            node->tree = new KDTree(emptySpaceCost_, maxNodeCost_);
            node->tree->Initialize(objects);
            return node;
        }
        if(rightMin.size() == 0)
        {
            node->leaf = true;
            std::vector<ISceneObject*> objects;
            for(auto it = leftMin.begin(); it != leftMin.end(); ++it)
                objects.push_back(it->second);
            node->tree = new KDTree(emptySpaceCost_, maxNodeCost_);
            node->tree->Initialize(objects);
            return node;
        }
        if(leftMin.size() == std::distance(minLeft, minRight) + 1 &&
                rightMin.size() == std::distance(minLeft, minRight) + 1)
        {
            node->leaf = true;
            std::vector<ISceneObject*> objects;
            for(auto it = leftMin.begin(); it != leftMin.end(); ++it)
                objects.push_back(it->second);
            node->tree = new KDTree(emptySpaceCost_, maxNodeCost_);
            node->tree->Initialize(objects);
            return node;
        }
        node->left = DivideAndBuild(axis, leftMax.begin(), leftMax.end() - 1, leftMin.begin(), leftMin.end() - 1, min, optimalSplit, depth + 1);
        node->right = DivideAndBuild(axis, rightMax.begin(), rightMax.end() - 1, rightMin.begin(), rightMin.end() - 1, optimalSplit, max, depth + 1);
        node->leftItems = leftMax.size();
        node->rightItems = rightMax.size();
    node->leaf = false;
    node->plane = optimalSplit;
    return node;
}

Color SHAKDTree::EmitLights(CollisionData& collision, std::vector<PointLight*>& lights)
{
#ifdef DEBUG
    if(collision.PixelColor.G > 0)
        return collision.PixelColor;
#endif
    Color c = collision.PixelColor.RGBtoHSV();
    c.B = 0;
    for(auto light: lights)
    {
        Photon photon(collision.CollisionPoint, light->GetPosition() - collision.CollisionPoint, collision.Owner);
        if(collision.CollisionNormal*photon.Direction() < 0 ||this->CollideNode(this->rootX_, this->rootY_, this->rootZ_, this->primaryBox_, photon)->IsCollide)
            continue;
        Color lColor = light->GetLight();
        lColor.B *= (light->GetIntensity() / std::pow((light->GetPosition() - collision.CollisionPoint).Length(),2))*
                (collision.CollisionNormal.Normalized()*photon.Direction().Normalized());
        c = c + lColor;
    }
    auto fin = c.HSVtoRGB();
    return fin;
}

CollisionData* SHAKDTree::RenderPhoton(Photon photon)
{
    Box box = this->primaryBox_;
    auto xNode = rootX_;
    auto yNode = rootY_;
    auto zNode = rootZ_;
    std::pair<Point, Point> intersects;
    if(!photon.IntersecWithBox(box, intersects))
        return new CollisionData(false);
    return this->CollideNode(xNode, yNode, zNode, box, photon);
}

float SHAKDTree::GetBoxValueMaxFromAxis(const Box& box, SHAKDTree::Axis axis) const
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
float SHAKDTree::GetBoxValueMinFromAxis(const Box& box, SHAKDTree::Axis axis) const
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

CollisionData*SHAKDTree::CollideNode(SHAKDTree::SHAKDNode* xNode, SHAKDTree::SHAKDNode* yNode, SHAKDTree::SHAKDNode* zNode,
                                  Box box, const Photon& photon)
{
    KDTree* tree;
    std::pair<Point, Point> intersects;
    std::pair<Point, Point> b1Intersects;
    std::pair<Point, Point> b2Intersects;
    bool xleaf = false;
    bool yleaf = false;
    bool zleaf = false;
    if(!photon.IntersecWithBox(box, intersects))
        return new CollisionData(false);
    if(xNode->leaf)
    {
        tree = xNode->tree;
        xleaf = true;
    }
    if(yNode->leaf)
    {
        if(!xleaf || (xleaf && yNode->tree->Size() < tree->Size()))
            tree = yNode->tree;
        yleaf = true;
    }
    if(zNode->leaf)
    {
        if((!xleaf && !yleaf)||zNode->tree->Size() < tree->Size())
            tree = zNode->tree;
        zleaf = true;
    }
    Axis maxAxis = A_X;
    auto length = box.XLength();
    if(box.YLength() > length)
    {
        maxAxis = A_Y;
        length = box.YLength();
    }
    if(box.ZLength() > length)
        maxAxis = A_Z;
    Box b1, b2;
    if((!xleaf || !yleaf || !zleaf))
    {
        if(!xleaf &&
                ((zleaf && yleaf)|| maxAxis == A_X))
        {
                b1 = box;
                b1.XMax = xNode->plane;
                b2 = box;
                b2.XMin = xNode->plane;
#ifdef DEBUG
                photon.IntersecWithBox(b1, intersects);
                if((IsFloatZero(intersects.first.X - xNode->plane) && IsFloatZero(intersects.first.Y - yNode->plane))
                        || (IsFloatZero(intersects.first.X - xNode->plane) && IsFloatZero(intersects.first.Z - zNode->plane))
                        || (IsFloatZero(intersects.first.Y - yNode->plane) && IsFloatZero(intersects.first.Z - zNode->plane)) )
                    return new CollisionData(true, Color(0, 1, 0), intersects.first, Point(1, 0, 0));
#endif
                if(!photon.IntersecWithBox(b1, b1Intersects))
                {
                    box = b2;
                    xNode = xNode->right;
                    return this->CollideNode(xNode, yNode, zNode, box, photon);
                }
                else
                {
                    if(!photon.IntersecWithBox(b2, b2Intersects))
                    {
                        box = b1;
                        xNode = xNode->left;
                        return this->CollideNode(xNode, yNode, zNode, box, photon);
                    }
                    else
                    {
                        CollisionData* collInfo;
                        photon.IntersecWithBox(box, intersects);
                        if(b1.IsInside(photon.Position()) || !photon.IntersecWithBox(b2, b2Intersects) ||
                                       ((photon.IntersecWithBox(b1, b1Intersects) &&
                                        (b1Intersects.first - photon.Position()).Length()  <= (b2Intersects.first - photon.Position()).Length())))
                        {
                            collInfo = this->CollideNode(xNode->left, yNode, zNode, b1, photon);
                            if(collInfo->IsCollide)// && b1.IsInside(collInfo->CollisionPoint))
                                return collInfo;
                            delete collInfo;
                            return this->CollideNode(xNode->right, yNode, zNode, b2, photon);
                        }
                        else
                        {
                            collInfo = this->CollideNode(xNode->right, yNode, zNode, b2, photon);
                            if(collInfo->IsCollide)// && b2.IsInside(collInfo->CollisionPoint))
                                return collInfo;
                            delete collInfo;
                            return this->CollideNode(xNode->left, yNode, zNode, b1, photon);
                        }
                    }
                }
            }
            else
            {
                if(!yleaf &&
                        ((xleaf && zleaf) || maxAxis == A_Y))
                {
                    b1 = box;
                    b1.YMax = yNode->plane;
                    b2 = box;
                    b2.YMin = yNode->plane;
#ifdef DEBUG
                    photon.IntersecWithBox(b1, intersects);
                    if((IsFloatZero(intersects.first.X - xNode->plane) && IsFloatZero(intersects.first.Y - yNode->plane))
                            || (IsFloatZero(intersects.first.X - xNode->plane) && IsFloatZero(intersects.first.Z - zNode->plane))
                            || (IsFloatZero(intersects.first.Y - yNode->plane) && IsFloatZero(intersects.first.Z - zNode->plane)) )
                        return new CollisionData(true, Color(0, 1, 0), intersects.first, Point(1, 0, 0));
#endif
                    if(!photon.IntersecWithBox(b1, b1Intersects))
                    {
                        box = b2;
                        yNode = yNode->right;
                        return this->CollideNode(xNode, yNode, zNode, box, photon);
                    }
                    else
                    {
                        if(!photon.IntersecWithBox(b2, b2Intersects))
                        {
                            box = b1;
                            yNode = yNode->left;
                            return this->CollideNode(xNode, yNode, zNode, box, photon);
                        }
                        else
                        {
                            CollisionData* collInfo;
                            photon.IntersecWithBox(box, intersects);
                            if(b1.IsInside(photon.Position()) || !photon.IntersecWithBox(b2, b2Intersects) ||
                                           ((photon.IntersecWithBox(b1, b1Intersects) &&
                                            (b1Intersects.first - photon.Position()).Length()  <= (b2Intersects.first - photon.Position()).Length())))
                            {
                                collInfo = this->CollideNode(xNode, yNode->left, zNode, b1, photon);
                                if(collInfo->IsCollide)// && b1.IsInside(collInfo->CollisionPoint))
                                    return collInfo;
                                delete collInfo;
                                return this->CollideNode(xNode, yNode->right, zNode, b2, photon);
                            }
                            else
                            {
                                collInfo = this->CollideNode(xNode, yNode->right, zNode, b2, photon);
                                if(collInfo->IsCollide)// && b2.IsInside(collInfo->CollisionPoint))
                                    return collInfo;
                                delete collInfo;
                                return this->CollideNode(xNode, yNode->left, zNode, b1, photon);
                            }
                        }
                    }
                }
                else if(!zleaf)
                {
                    b1 = box;
                    b1.ZMax = zNode->plane;
                    b2 = box;
                    b2.ZMin = zNode->plane;
#ifdef DEBUG
                    photon.IntersecWithBox(b1, intersects);
                    if((IsFloatZero(intersects.first.X - xNode->plane) && IsFloatZero(intersects.first.Y - yNode->plane))
                            || (IsFloatZero(intersects.first.X - xNode->plane) && IsFloatZero(intersects.first.Z - zNode->plane))
                            || (IsFloatZero(intersects.first.Y - yNode->plane) && IsFloatZero(intersects.first.Z - zNode->plane)) )
                        return new CollisionData(true, Color(0, 1, 0), intersects.first, Point(1, 0, 0));
#endif

                    if(!photon.IntersecWithBox(b1, b1Intersects))
                    {
                        box = b2;
                        zNode = zNode->right;
                        return this->CollideNode(xNode, yNode, zNode, box, photon);
                    }
                    else
                    {
                        if(!photon.IntersecWithBox(b2, b2Intersects))
                        {
                            box = b1;
                            zNode = zNode->left;
                            return this->CollideNode(xNode, yNode, zNode, box, photon);
                        }
                        else
                        {
                            CollisionData* collInfo;
                            photon.IntersecWithBox(box, intersects);
                            if(b1.IsInside(photon.Position()) || !photon.IntersecWithBox(b2, b2Intersects) ||
                                           ((photon.IntersecWithBox(b1, b1Intersects) &&
                                            (b1Intersects.first - photon.Position()).Length()  <= (b2Intersects.first - photon.Position()).Length())))
                            {
                                collInfo = this->CollideNode(xNode, yNode, zNode->left, b1, photon);
                                if(collInfo->IsCollide)// && b1.IsInside(collInfo->CollisionPoint))
                                    return collInfo;
                                delete collInfo;
                                return this->CollideNode(xNode, yNode, zNode->right, b2, photon);
                            }
                            else
                            {
                                collInfo = this->CollideNode(xNode, yNode, zNode->right, b2, photon);
                                if(collInfo->IsCollide)// && b2.IsInside(collInfo->CollisionPoint))
                                    return collInfo;
                                delete collInfo;
                                return this->CollideNode(xNode, yNode, zNode->left, b1, photon);
                            }
                        }
                    }
                }
            }
        }
    return tree->CollidePhoton(photon);
}

bool SHAKDTree::CheckCollide(SHAKDTree::SHAKDNode* xNode, SHAKDTree::SHAKDNode* yNode, SHAKDTree::SHAKDNode* zNode, Box box, const Photon& photon)
{
    KDTree* tree;
    std::pair<Point, Point> intersects;
    std::pair<Point, Point> b1Intersects;
    std::pair<Point, Point> b2Intersects;
    bool xleaf = false;
    bool yleaf = false;
    bool zleaf = false;
    if(!photon.IntersecWithBox(box, intersects))
        return false;
    if(xNode->leaf)
    {
        tree = xNode->tree;
        xleaf = true;
    }
    if(yNode->leaf)
    {
        if(!xleaf || (xleaf && yNode->tree->Size() < tree->Size()))
            tree = yNode->tree;
        yleaf = true;
    }
    if(zNode->leaf)
    {
        if((!xleaf && !yleaf)||zNode->tree->Size() < tree->Size())
            tree = zNode->tree;
        zleaf = true;
    }
    Axis maxAxis = A_X;
    auto length = box.XLength();
    if(box.YLength() > length)
    {
        maxAxis = A_Y;
        length = box.YLength();
    }
    if(box.ZLength() > length)
        maxAxis = A_Z;
    Box b1, b2;
    if((!xleaf || !yleaf || !zleaf))
    {
        if(!xleaf &&
                ((zleaf && yleaf)|| maxAxis == A_X))
        {
                b1 = box;
                b1.XMax = xNode->plane;
                b2 = box;
                b2.XMin = xNode->plane;
#ifdef DEBUG
                photon.IntersecWithBox(b1, intersects);
                if((IsFloatZero(intersects.first.X - xNode->plane) && IsFloatZero(intersects.first.Y - yNode->plane))
                        || (IsFloatZero(intersects.first.X - xNode->plane) && IsFloatZero(intersects.first.Z - zNode->plane))
                        || (IsFloatZero(intersects.first.Y - yNode->plane) && IsFloatZero(intersects.first.Z - zNode->plane)) )
                    return new CollisionData(true, Color(0, 1, 0), intersects.first, Point(1, 0, 0));
#endif
                if(!photon.IntersecWithBox(b1, b1Intersects))
                {
                    box = b2;
                    xNode = xNode->right;
                    return this->CheckCollide(xNode, yNode, zNode, box, photon);
                }
                else
                {
                    if(!photon.IntersecWithBox(b2, b2Intersects))
                    {
                        box = b1;
                        xNode = xNode->left;
                        return this->CheckCollide(xNode, yNode, zNode, box, photon);
                    }
                    else
                    {
                        photon.IntersecWithBox(box, intersects);
                        if(b1.IsInside(photon.Position()) || !photon.IntersecWithBox(b2, b2Intersects) ||
                                       ((photon.IntersecWithBox(b1, b1Intersects) &&
                                        (b1Intersects.first - photon.Position()).Length()  <= (b2Intersects.first - photon.Position()).Length())))
                        {
                            auto collInfo = this->CheckCollide(xNode->left, yNode, zNode, b1, photon);
                            if(collInfo)
                                return collInfo;
                            return this->CheckCollide(xNode->right, yNode, zNode, b2, photon);
                        }
                        else
                        {
                            auto collInfo = this->CheckCollide(xNode->right, yNode, zNode, b2, photon);
                            if(collInfo)
                                return collInfo;
                            return this->CheckCollide(xNode->left, yNode, zNode, b1, photon);
                        }
                    }
                }
            }
            else
            {
                if(!yleaf &&
                        ((xleaf && zleaf) || maxAxis == A_Y))
                {
                    b1 = box;
                    b1.YMax = yNode->plane;
                    b2 = box;
                    b2.YMin = yNode->plane;
                    if(!photon.IntersecWithBox(b1, b1Intersects))
                    {
                        box = b2;
                        yNode = yNode->right;
                        return this->CheckCollide(xNode, yNode, zNode, box, photon);
                    }
                    else
                    {
                        if(!photon.IntersecWithBox(b2, b2Intersects))
                        {
                            box = b1;
                            yNode = yNode->left;
                            return this->CheckCollide(xNode, yNode, zNode, box, photon);
                        }
                        else
                        {
                            photon.IntersecWithBox(box, intersects);
                            if(b1.IsInside(photon.Position()) || !photon.IntersecWithBox(b2, b2Intersects) ||
                                           ((photon.IntersecWithBox(b1, b1Intersects) &&
                                            (b1Intersects.first - photon.Position()).Length()  <= (b2Intersects.first - photon.Position()).Length())))
                            {
                                auto collInfo = this->CheckCollide(xNode, yNode->left, zNode, b1, photon);
                                if(collInfo)
                                    return collInfo;
                                return this->CheckCollide(xNode, yNode->right, zNode, b2, photon);
                            }
                            else
                            {
                                auto collInfo = this->CheckCollide(xNode, yNode->right, zNode, b2, photon);
                                if(collInfo)
                                    return collInfo;
                                return this->CheckCollide(xNode, yNode->left, zNode, b1, photon);
                            }
                        }
                    }
                }
                else if(!zleaf)
                {
                    b1 = box;
                    b1.ZMax = zNode->plane;
                    b2 = box;
                    b2.ZMin = zNode->plane;
                    if(!photon.IntersecWithBox(b1, b1Intersects))
                    {
                        box = b2;
                        zNode = zNode->right;
                        return this->CheckCollide(xNode, yNode, zNode, box, photon);
                    }
                    else
                    {
                        if(!photon.IntersecWithBox(b2, b2Intersects))
                        {
                            box = b1;
                            zNode = zNode->left;
                            return this->CheckCollide(xNode, yNode, zNode, box, photon);
                        }
                        else
                        {
                            photon.IntersecWithBox(box, intersects);
                            if(b1.IsInside(photon.Position()) || !photon.IntersecWithBox(b2, b2Intersects) ||
                                           ((photon.IntersecWithBox(b1, b1Intersects) &&
                                            (b1Intersects.first - photon.Position()).Length()  <= (b2Intersects.first - photon.Position()).Length())))
                            {
                                auto collInfo = this->CheckCollide(xNode, yNode, zNode->left, b1, photon);
                                if(collInfo)
                                    return collInfo;
                                return this->CheckCollide(xNode, yNode, zNode->right, b2, photon);
                            }
                            else
                            {
                                auto collInfo = this->CheckCollide(xNode, yNode, zNode->right, b2, photon);
                                if(collInfo)
                                    return collInfo;
                                return this->CheckCollide(xNode, yNode, zNode->left, b1, photon);
                            }
                        }
                    }
                }
            }
        }
    return tree->CheckCollide(photon);
}
*/
