#ifndef PHOTON_H
#define PHOTON_H

#include "Base.h"

class Photon
{
    public:
        Photon(Point position, Point direction);
        Point Position();
        Point Direction();
    private:
        Point position_;
        Point direction_;
};

#endif
