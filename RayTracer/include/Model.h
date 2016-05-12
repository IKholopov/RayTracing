#ifndef MODEL_H
#define MODEL_H

#include "ISceneObject.h"
#include "IMaterial.h"
#include "Transform.h"

class Model
{
    public:
        Model();
        Model(std::vector<ISceneObject*> polygons);
        std::vector<ISceneObject*>& GetGeometry();
        void ApplyTransformation(Transform A);
        void ApplyMaterial(IMaterial* material);
    private:
        std::vector<ISceneObject*> polygons_;
};

#endif
