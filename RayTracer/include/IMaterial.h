#ifndef I_MATERIAL_H
#define I_MATERIAL_H

#include <vector>

#include "Base.h"
#include "Photon.h"

class IMaterial
{
    public:
        virtual ~IMaterial() {}
        virtual void GetNextPhotons(std::vector<Photon>& photons) = 0;
        virtual Color GetSelfColor() = 0;
};

#endif
