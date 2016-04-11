#ifndef PHOTON_H
#define PHOTON_H

#include "utility"

#include "Base.h"

class Photon
{
    public:
        Photon(Point position, Point direction);
        Point Position() const;
        Point Direction() const;
        bool IntersecWithBox(const Box& box, std::pair<Point, Point>& points) const;
    private:
        Point position_;
        Point direction_;
};

#endif
