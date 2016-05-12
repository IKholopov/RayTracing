#ifndef STL_BINARY_SERIALIZER_H
#define STL_BINARY_SERIALIZER_H

#include "ISerializer.h"
#include "Model.h"

class STLBinarySerializer : public ISerializer
{
    public:
        Scene* LoadScene(std::string filepath, IView* view);
        Model* LoadModel(std::string filepath);
};

#endif
