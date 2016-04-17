#ifndef PHOTON_H
#define PHOTON_H

#include "utility"

#include "Base.h"

#include "ISceneObject.h"

class ISceneObject;

class Photon
{
    public:
        Photon(Point position, Point direction, ISceneObject* owner = nullptr);
        Point Position() const;
        Point Direction() const;
        ISceneObject* Owner() const;
        bool IntersecWithBox(const Box& box, std::pair<Point, Point>& points) const;
    private:
        Point position_;
        Point direction_;
        ISceneObject* owner_;
};

#endif
