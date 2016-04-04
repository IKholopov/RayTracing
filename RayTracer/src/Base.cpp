#include "Base.h"

Resolution::Resolution(unsigned int width, unsigned int height):Width(width), Height(height)
{}

Color::Color(float r, float g, float b): R(r), G(g), B(b)
{}

Point::Point(float x, float y, float z):X(x), Y(y), Z(z)
{}

Box::Box(Point x1y1z1, Point x1y1z2, Point x1y2z1, Point x1y2z2,
         Point x2y1z1, Point x2y1z2, Point x2y2z1, Point x2y2z2):x1y1z1(x1y1z1), x1y1z2(x1y1z2), x1y2z1(x1y2z1), x1y2z2(x1y2z2),
        x2y1z1(x2y1z1), x2y1z2(x2y1z2), x2y2z1(x2y2z1),x2y2z2(x2y2z2)
{}

