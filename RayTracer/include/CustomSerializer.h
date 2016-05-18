#ifndef CUSTOM_SERIALIZER_H
#define CUSTOM_SERIALIZER_H

#include "ISerializer.h"
#include "Model.h"

class CustomSerializer : public ISerializer
{
    public:
        Scene* LoadScene(std::string filepath, IView* view);
        void ExportScene(std::string filepath, Scene* scene);
};

#endif
