#ifndef SHA_TREE_H
#define SHA_TREE_H

#include "ISceneObject.h"
#include "IGeometryHierarchy.h"
#include "PointLight.h"

class SHATree: public IGeometryHierarchy
{
    public:
        SHATree(float emptySpaceCost, float maxNodeCost, unsigned int maxDepth);
        void Initialize(std::vector<ISceneObject*>& objects);
    private:
        enum Axis{ A_X, A_Y, A_Z};
        struct SHANode{
            SHANode(Axis axis): axis(axis) {}
            Axis axis;
            float plane;
            unsigned int leftItems;
            unsigned int rightItems;
            bool leaf;
            SHANode* left;
            SHANode* right;
            std::vector<ISceneObject*> object;
        };

        SHANode* DivideAndBuild(Axis axis, std::vector<std::pair<Box, ISceneObject*>>::const_iterator maxLeft,
                                std::vector<std::pair<Box, ISceneObject*>>::const_iterator maxRight,
                                std::vector<std::pair<Box, ISceneObject*>>::const_iterator minLeft,
                                std::vector<std::pair<Box, ISceneObject*>>::const_iterator minRight,
                               float min, float max, unsigned int depth);
        Color EmitLights(CollisionData& collision, std::vector<PointLight*>& lights);
        Color RenderPhoton(Photon photon, std::vector<PointLight*>& lights);

        float GetBoxValueMaxFromAxis(const Box& box, Axis axis) const;
        float GetBoxValueMinFromAxis(const Box& box, Axis axis) const;
        CollisionData* CollideNode(SHANode* xNode, SHANode* yNode, SHANode* zNode,
                                   Box box, const Photon& photon);
        SHANode* rootX_;
        SHANode* rootY_;
        SHANode* rootZ_;
        Box primaryBox_;
        float emptySpaceCost_;
        float maxNodeCost_;
        unsigned int maxDepth_;

};

#endif
