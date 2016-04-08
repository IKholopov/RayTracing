#include "SimpleMaterial.h"

SimpleMaterial::SimpleMaterial(Color color):color_(color)
{

}

void SimpleMaterial::GetNextPhotons(std::vector<Photon>& photons)
{
    return;
}

Color SimpleMaterial::GetSelfColor()
{
    return color_;
}
