#ifndef K_D_TREE_H
#define K_D_TREE_H

#include "ISceneObject.h"
#include "IGeometryHierarchy.h"
#include "PointLight.h"

class KDFairTree: public IGeometryHierarchy
{
    public:
        KDFairTree(int emptySpaceCost);
        void Initialize(std::vector<ISceneObject*>& objects);
        CollisionData* CollidePhoton(const Photon& photon);
        bool CheckCollide(const Photon& photon);
        unsigned int Size();
    private:
        struct KDFairNode{
            KDFairNode() {}
            float plane;
            bool leaf;
            KDFairNode* left;
            KDFairNode* right;
            Box box;
            Axis axis;
            size_t size;
            std::vector<ISceneObject*> objects;
        };

        KDFairNode* DivideAndBuild(std::vector<ISceneObject*>& objects, Box box, int depth);
        CollisionData* RenderPhoton(Photon photon);
        CollisionData* CollideNode(KDFairNode* node, const Photon& photon);
        //bool CheckCollide(KDFairNode* node, const Photon& photon);
        KDFairNode* root_;
        Box primaryBox_;
        float emptySpaceCost_;
        int depth_;
        unsigned int size_;
};

#endif
