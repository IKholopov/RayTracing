#include "Camera.h"

Camera::Camera(Point point, unsigned int width, unsigned int height, Plane screen):viewpoint_(point),
                                    resolution_(Resolution(width, height)), screen_(screen)
{

}

Camera::Camera(Point point, Plane screen):viewpoint_(point),
    resolution_(Resolution(0, 0)), screen_(screen)
{

}

unsigned int Camera::GetWidth()
{
    return resolution_.Width;
}

unsigned int Camera::GetHeight()
{
    return resolution_.Height;
}

Photon Camera::GetPhotonForPixel(unsigned int x, unsigned int y)
{
    auto xEdge = (screen_.x4y4z4 - screen_.x3y3z3)*float(x)*(1.0/resolution_.Width);
    auto yEdge = ((screen_.x1y1z1 - screen_.x3y3z3)*float(y)*(1.0/resolution_.Height));
    auto screenPos = screen_.x3y3z3 + xEdge + yEdge;
    return Photon(viewpoint_, screenPos - viewpoint_);
}

void Camera::SetResolution(Resolution resolution)
{
    this->resolution_ = resolution;
}

Plane Camera::screen() const
{
    return screen_;
}

Point Camera::GetViewpoint() const
{
    return viewpoint_;
}

Plane Camera::GetScreen() const
{
    return this->screen();
}
