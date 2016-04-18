#ifndef K_D_TREE_H
#define K_D_TREE_H

#include "ISceneObject.h"
#include "IGeometryHierarchy.h"
#include "PointLight.h"

class KDTree: public IGeometryHierarchy
{
    public:
        KDTree(float emptySpaceCost, float maxNodeCost);
        void Initialize(std::vector<ISceneObject*>& objects);
    private:
        struct KDNode{
            KDNode() {}
            float plane;
            bool leaf;
            KDNode* left;
            KDNode* right;
            Box box;
            Axis axis;
            std::vector<ISceneObject*> objects;
        };

        KDNode* DivideAndBuild(std::vector<std::pair<Box, ISceneObject*> >& objects, Box box);
        Color EmitLights(CollisionData& collision, std::vector<PointLight*>& lights);
        Color RenderPhoton(Photon photon, std::vector<PointLight*>& lights);

        float GetBoxValueMaxFromAxis(const Box& box, Axis axis) const;
        float GetBoxValueMinFromAxis(const Box& box, Axis axis) const;
        CollisionData* CollideNode(KDNode* node, const Photon& photon);
        KDNode* root_;
        Box primaryBox_;
        float emptySpaceCost_;
        float maxNodeCost_;

};

#endif
