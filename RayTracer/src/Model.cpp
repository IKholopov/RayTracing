#include "Model.h"

Model::Model()
{

}

Model::Model(std::vector<ISceneObject*> polygons):polygons_(polygons)
{

}

std::vector<ISceneObject*>&Model::GetGeometry()
{
    return polygons_;
}

void Model::ApplyTransformation(Transform A)
{
    for(auto obj: this->GetGeometry())
    {
        obj->ApplyTransform(A);
    }
}

void Model::ApplyMaterial(IMaterial* material)
{
    for(auto obj: polygons_)
    {
        obj->ApplyOutterMaterial(material);
        obj->ApplyInnerMaterial(material);
    }
}
