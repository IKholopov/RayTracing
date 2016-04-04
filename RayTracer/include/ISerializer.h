#ifndef I_SERIALIZER_H
#define I_SERIALIZER_H

#include <string>

#include "Scene.h"

class ISerializer
{
    public:
        virtual ~ISerializer() = 0;
        Scene LoadScene(std::string filepath) = 0;
};

#endif
