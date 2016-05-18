#ifndef SIMPLE_SERIALIZER_H
#define SIMPLE_SERIALIZER_H

#include "ISerializer.h"

class SimpleSerializer : public ISerializer
{
    public:
        Scene* LoadScene(std::string filepath, IView* view);
        void ExportScene(std::string filepath, Scene* scene);
    private:
        IMaterial* LoadMaterial(std::ifstream& stream);
};

#endif
