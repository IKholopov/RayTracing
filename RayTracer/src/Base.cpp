#include "Base.h"

#include <cmath>

Resolution::Resolution(unsigned int width, unsigned int height):Width(width), Height(height)
{}

Color::Color():R(0), G(0), B(0)
{}
Color::Color(float r, float g, float b): R(r), G(g), B(b)
{}


Point::Point():X(0), Y(0), Z(0)
{}
Point::Point(float x, float y, float z):X(x), Y(y), Z(z)
{}

float Point::Length()
{
    return std::sqrt(X*X+Y*Y+Z*Z);
}

Point Point::Normalized()
{
    auto l = this->Length();
    return Point(X / l, Y / l, Z / l);
}

Point Point::operator + (Point& a)
{
    return Point(a.X + this->X, a.Y + this->Y, a.Z + this->Z);
}
Point Point::operator + (Point&& a)
{
    return Point(a.X + this->X, a.Y + this->Y, a.Z + this->Z);
}

Point Point::operator *(float a)
{
    return Point(this->X*a, this->Y*a, this->Z*a);
}
Point Point::operator *(double a)
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

float Point::operator *(const Point& b)
{
    return this->X * b.X + this->Y * b.Y + this->Z * b.Z;
}
float Point::operator *(Point&& b)
{
    return this->X * b.X + this->Y * b.Y + this->Z * b.Z;
}

Point Point::operator - (Point& b)
{
    return Point(this->X - b.X, this->Y - b.Y, this->Z - b.Z);
}
Point Point::operator - (Point&& b)
{
    return Point(this->X - b.X, this->Y - b.Y, this->Z - b.Z);
}



Plane::Plane(Point x1y1z1, Point x2y2z2, Point x3y3z3, Point x4y4z4) : x1y1z1(x1y1z1), x2y2z2(x2y2z2), x3y3z3(x3y3z3), x4y4z4(x4y4z4)
{
}

Box::Box():XMax(0), XMin(0), YMax(0), YMin(0), ZMax(0), ZMin(0)
{}

Box::Box(float xMax, float xMin, float yMax, float yMin, float zMax, float zMin):XMax(xMax), XMin(xMin),
    YMax(yMax), YMin(yMin), ZMax(zMax), ZMin(zMin)
{}

float Box::XLength()
{
    return XMax - XMin;
}

float Box::YLength()
{
    return YMax - YMin;
}

float Box::ZLength()
{
    return ZMax - ZMin;
}

bool IsFloatZero(float a)
{
    if(a < 1e-6)
        return true;
}
