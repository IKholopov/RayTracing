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
        Photon(Point position, Point direction, float minDist, ISceneObject* owner = nullptr);
        Point Position() const;
        Point Direction() const;
        float GetMinDist() const;
        ISceneObject* Owner() const;
        bool IntersecWithBox(const Box& box) const;
    private:
        Point position_;
        Point direction_;
        float minDist_;
        ISceneObject* owner_;
};

#endif
