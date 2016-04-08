#ifndef K_D_TREE_H
#define K_D_TREE_H

#include "ISceneObject.h"

class KDTree
{
    public:
        KDTree(std::vector<ISceneObject*>& objects);
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

        KDNode* DivideAndBuild(Axis x, std::vector<std::pair<Box, ISceneObject*>>::const_iterator maxLeft,
                                std::vector<std::pair<Box, ISceneObject*>>::const_iterator maxRight,
                                std::vector<std::pair<Box, ISceneObject*>>::const_iterator minLeft,
                                std::vector<std::pair<Box, ISceneObject*>>::const_iterator minRight);

        KDNode* rootX_;
        KDNode* rootY_;
        KDNode* rootZ_;

};

#endif
