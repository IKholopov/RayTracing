#include "Base.h"

Resolution::Resolution(unsigned int width, unsigned int height):Width(width), Height(height)
{}

Color::Color(float r, float g, float b): R(r), G(g), B(b)
{}


Point::Point(float x, float y, float z):X(x), Y(y), Z(z)
{}

Point Point::operator + (Point& a)
{
    return Point(a.X + this->X, a.Y + this->Y, a.Z + this->Z);
}

Point Point::operator *(float a)
{
    return Point(this->X*a, this->Y*a, this->Z*a);
}
Point Point::operator *(int a)
{
    return Point(this->X*a, this->Y*a, this->Z*a);
}
Point Point::operator *(unsigned int a)
{
    return Point(this->X*a, this->Y*a, this->Z*a);
}

Point Point::operator - (Point& b)
{
    return Point(this->X - b.X, this->Y - b.Y, this->Z - b.Z);
}



Plane::Plane(Point x1y1z1, Point x2y2z2, Point x3y3z3, Point x4y4z4) : x1y1z1(x1y1z1), x2y2z2(x2y2z2), x3y3z3(x3y3z3), x4y4z4(x4y4z4)
{
}

Box::Box(Point x1y1z1, Point x1y1z2, Point x1y2z1, Point x1y2z2,
         Point x2y1z1, Point x2y1z2, Point x2y2z1, Point x2y2z2):x1y1z1(x1y1z1), x1y1z2(x1y1z2), x1y2z1(x1y2z1), x1y2z2(x1y2z2),
        x2y1z1(x2y1z1), x2y1z2(x2y1z2), x2y2z1(x2y2z1),x2y2z2(x2y2z2)
{}


