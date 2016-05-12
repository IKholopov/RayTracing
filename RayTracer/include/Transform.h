#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Base.h"

class Transform
{
    public:
        Transform(Point x, Point y, Point z, Point W);
        Point Apply(Point a);

        static Transform Move(Point move);
    private:
        Point X;
        Point Y;
        Point Z;
        Point W;
};

#endif

