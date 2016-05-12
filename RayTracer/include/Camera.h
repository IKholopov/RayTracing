#ifndef CAMERA_H
#define CAMERA_H

#include "Base.h"
#include "Photon.h"

class Camera
{
    public:
        Camera(Point point, unsigned int width, unsigned int height, Plane screen);
        Camera(Point point, Plane screen);
        unsigned int GetWidth();
        unsigned int GetHeight();
        Photon GetPhotonForPixel(unsigned int x, unsigned int y);
        void SetResolution(Resolution resolution);
    private:
        Point viewpoint_;
        Plane screen_;
        Resolution resolution_;
};

#endif
