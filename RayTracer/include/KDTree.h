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
        CollisionData* CollidePhoton(const Photon& photon);
        bool CheckCollide(const Photon& photon);
        unsigned int Size();
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
        CollisionData* RenderPhoton(Photon photon);

        float GetBoxValueMaxFromAxis(const Box& box, Axis axis) const;
        float GetBoxValueMinFromAxis(const Box& box, Axis axis) const;
        CollisionData* CollideNode(KDNode* node, const Photon& photon);
        bool CheckCollide(KDNode* node, const Photon& photon);
        KDNode* root_;
        Box primaryBox_;
        float emptySpaceCost_;
        float maxNodeCost_;
        unsigned int size_;
        IGeometryHierarchy* primeHierarchy;

};

#endif
