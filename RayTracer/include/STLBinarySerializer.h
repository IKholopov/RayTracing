#ifndef STL_BINARY_SERIALIZER_H
#define STL_BINARY__SERIALIZER_H

#include "ISerializer.h"

class STLBinarySerializer : public ISerializer
{
    public:
        Scene* LoadScene(std::string filepath, IView* view);
};

#endif
