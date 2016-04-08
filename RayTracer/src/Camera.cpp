#include "Camera.h"

Camera::Camera(Point point, unsigned int width, unsigned int height, Plane screen):viewpoint_(point),
                                    resolution_(Resolution(width, height)), screen_(screen)
{

}

Photon Camera::GetPhotonForPixel(unsigned int x, unsigned int y)
{
    auto xEdge = (screen_.x2y2z2 - screen_.x1y1z1)*x;
    auto yEdge = ((screen_.x3y3z3 - screen_.x1y1z1)*y);
    auto screenPos = screen_.x1y1z1 + xEdge + yEdge;
    return Photon(screenPos, screenPos - viewpoint_);
}
