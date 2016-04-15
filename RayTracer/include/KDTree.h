#ifndef K_D_TREE_H
#define K_D_TREE_H

#include "ISceneObject.h"
#include "IGeometryHierarchy.h"

class KDTree: public IGeometryHierarchy
{
    public:
        KDTree(float emptySpaceCost, float maxNodeCost, unsigned int maxDepth);
        void Initialize(std::vector<ISceneObject*>& objects);
    private:
        enum Axis{ A_X, A_Y, A_Z};
        struct KDNode{
            KDNode(Axis axis): axis(axis) {}
            Axis axis;
            float plane;
            unsigned int leftItems;
            unsigned int rightItems;
            bool leaf;
            KDNode* left;
            KDNode* right;
            std::vector<ISceneObject*> object;
        };

        KDNode* DivideAndBuild(Axis axis, std::vector<std::pair<Box, ISceneObject*>>::const_iterator maxLeft,
                                std::vector<std::pair<Box, ISceneObject*>>::const_iterator maxRight,
                                std::vector<std::pair<Box, ISceneObject*>>::const_iterator minLeft,
                                std::vector<std::pair<Box, ISceneObject*>>::const_iterator minRight,
                               float min, float max, unsigned int depth);

        Color RenderPhoton(Photon photon);

        float GetBoxValueMaxFromAxis(const Box& box, Axis axis) const;
        float GetBoxValueMinFromAxis(const Box& box, Axis axis) const;
        CollisionData* CollideNode(KDNode* xNode, KDNode* yNode, KDNode* zNode,
                                   Box box, const Photon& photon);
        KDNode* rootX_;
        KDNode* rootY_;
        KDNode* rootZ_;
        Box primaryBox_;
        float emptySpaceCost_;
        float maxNodeCost_;
        unsigned int maxDepth_;

};

#endif
