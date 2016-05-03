#ifndef SHA_KD_TREE_H
#define SHA_KD_TREE_H

#include "ISceneObject.h"
#include "IGeometryHierarchy.h"
#include "PointLight.h"
#include "KDTree.h"

class SHAKDTree: public IGeometryHierarchy
{
    public:
        SHAKDTree(float emptySpaceCost, float maxNodeCost, unsigned int maxDepth);
        void Initialize(std::vector<ISceneObject*>& objects);
    private:
        enum Axis{ A_X, A_Y, A_Z};
        struct SHAKDNode{
            SHAKDNode(Axis axis): axis(axis) {}
            Axis axis;
            float plane;
            unsigned int leftItems;
            unsigned int rightItems;
            bool leaf;
            SHAKDNode* left;
            SHAKDNode* right;
            KDTree* tree;
        };

        SHAKDNode* DivideAndBuild(Axis axis, std::vector<std::pair<Box, ISceneObject*>>::const_iterator maxLeft,
                                std::vector<std::pair<Box, ISceneObject*>>::const_iterator maxRight,
                                std::vector<std::pair<Box, ISceneObject*>>::const_iterator minLeft,
                                std::vector<std::pair<Box, ISceneObject*>>::const_iterator minRight,
                               float min, float max, unsigned int depth);
        Color EmitLights(CollisionData& collision, std::vector<PointLight*>& lights);
        CollisionData* RenderPhoton(Photon photon);

        float GetBoxValueMaxFromAxis(const Box& box, Axis axis) const;
        float GetBoxValueMinFromAxis(const Box& box, Axis axis) const;
        CollisionData* CollideNode(SHAKDNode* xNode, SHAKDNode* yNode, SHAKDNode* zNode,
                                   Box box, const Photon& photon);
        bool CheckCollide(SHAKDNode* xNode, SHAKDNode* yNode, SHAKDNode* zNode,
                                   Box box, const Photon& photon);
        SHAKDNode* rootX_;
        SHAKDNode* rootY_;
        SHAKDNode* rootZ_;
        Box primaryBox_;
        float emptySpaceCost_;
        float maxNodeCost_;
        unsigned int maxDepth_;

};

#endif
