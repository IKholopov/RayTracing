#ifndef CAMERA_H
#define CAMERA_H

#include "Base.h"
#include "Photon.h"

class Camera
{
    public:
        Camera(Point point, unsigned int width, unsigned int height, Plane screen);
    private:
        Point viewpoint_;
        Plane screen_;
        Resolution resolution_;
        Photon GetPhotonForPixel(unsigned int x, unsigned int y);
};

#endif
