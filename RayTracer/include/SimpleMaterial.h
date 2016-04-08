#ifndef SIMPLE_MATERIAL_H
#define SIMPLE_MATERIAL_H

#include "IMaterial.h"

class SimpleMaterial:public IMaterial
{
    public:
        SimpleMaterial(Color color);
        void GetNextPhotons(std::vector<Photon>& photons);
        Color GetSelfColor();

    private:
        Color color_;

};

#endif
