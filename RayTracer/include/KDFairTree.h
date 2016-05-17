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
        CollisionData* CheckCollide(const Photon& photon);
        unsigned int Size();

    private:
        class SortedArrays
        {
            public:
                SortedArrays();
                SortedArrays(std::vector<ISceneObject*>& objects);
                std::vector<ISceneObject*> XMax;
                std::vector<ISceneObject*> XMin;
                std::vector<ISceneObject*> YMax;
                std::vector<ISceneObject*> YMin;
                std::vector<ISceneObject*> ZMax;
                std::vector<ISceneObject*> ZMin;

                void Split(Axis axis, float plane, SortedArrays& array1, SortedArrays& array2);
                std::vector<ISceneObject*>& GetSortedMax(Axis axis);
                std::vector<ISceneObject*>& GetSortedMin(Axis axis);
        };
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

        CollisionData*CheckCollideNode(KDFairTree::KDFairNode* node, const Photon& photon);
        KDFairNode* DivideAndBuild(SortedArrays& objects, Box box, int depth, Axis axis, bool divideByMax);
        //KDFairNode* DivideAndBuild(std::vector<ISceneObject*>& objects, Box box, int depth, Axis axis, bool divideByMax);
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
