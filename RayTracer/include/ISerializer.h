#ifndef I_SERIALIZER_H
#define I_SERIALIZER_H

#include <string>

#include "Scene.h"
#include "IView.h"

class ISerializer
{
    public:
        virtual ~ISerializer() {};
        virtual Scene* LoadScene(std::string filepath, IView* view) = 0;
        virtual void ExportScene(std::string filepath, Scene* scene) = 0;
};

#endif
