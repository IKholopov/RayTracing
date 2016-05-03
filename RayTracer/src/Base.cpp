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

float Point::GetAxis(Axis axis)
{
    if(axis == A_X)
        return X;
    if(axis == A_Y)
        return Y;
    return Z;
}

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

Point Point::operator ^(const Point& b)
{
    return Point(this->Y * b.Z - b.Y * this->Z,
                 this->Z * b.X - b.Z * this->X,
                 this->X * b.Y - b.X * this->Y);
}
Point Point::operator ^(Point&& b)
{
    return Point(this->Y * b.Z - b.Y * this->Z,
                 this->Z * b.X - b.Z * this->X,
                 this->X * b.Y - b.X * this->Y);
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

float Box::GetMid(Axis axis)
{
    if(axis == A_X)
        return (XMax + XMin) / 2;
    if(axis == A_Y)
        return (YMax + YMin) / 2;
    return (ZMax + ZMin) / 2;
}

float Box::GetMax(Axis axis)
{
    if(axis == A_X)
        return XMax;
    if(axis == A_Y)
        return YMax;
    return ZMax;
}

float Box::GetMin(Axis axis)
{
    if(axis == A_X)
        return XMin;
    if(axis == A_Y)
        return YMin;
    return ZMin;
}

void Box::SetAxisMin(Axis axis, float val)
{
    if(axis == A_X)
        this->XMin = val;
    else if(axis == A_Y)
        this->YMin = val;
    else
        this->ZMin = val;
}
void Box::SetAxisMax(Axis axis, float val)
{
    if(axis == A_X)
        this->XMax = val;
    else if(axis == A_Y)
        this->YMax = val;
    else
        this->ZMax = val;
}
float Box::YLength()
{
    return YMax - YMin;
}

float Box::ZLength()
{
    return ZMax - ZMin;
}

bool Box::IsInside(Point point)
{
    if((point.X < XMax || IsFloatZero(point.X - XMax)) && (point.X >= XMin  || IsFloatZero(point.X - XMin)) &&
            (point.Y <= YMax  || IsFloatZero(point.Y - YMax)) && (point.Y >= YMin  || IsFloatZero(point.Y - YMin))&&
            (point.Z <= ZMax  || IsFloatZero(point.Z - ZMax)) && (point.Z >= ZMin  || IsFloatZero(point.Z - ZMin)))
        return true;
    return false;
}

bool IsFloatZero(float a)
{
    if(std::fabs(a) < 1e-4)
        return true;
    return false;
}

Color Color::RGBtoHSV() {
    float maxComp = std::max(std::max(R, G), B);
    float minComp = std::min(std::min(R, G), B);
    float delta = maxComp - minComp;
    float H, S, V;
    if(delta > 0) {
        if(maxComp == R) {
            H = 60 * std::fmod((G-B)/delta, 6);
        } else if(maxComp == G) {
            H = 60 * (((B - R) / delta) + 2);
        } else if(maxComp == B) {
            H = 60 * (((R - G) / delta) + 4);
        }

        if(maxComp > 0) {
            S = delta / maxComp;
        } else {
            S = 0;
        }
    } else {
        H = 0;
        S = 0;
    }
    V = maxComp;

    if(H < 0) {
        H +-360;
    }
    return Color(H, S, V);
}

Color Color::HSVtoRGB()
{
    float C = G*B;
    float X = C*(1-std::fabs(std::fmod(R/60.0f, 2) -1));
    float m = B - C;
    Color result;
    if(R < 120.0)
    {
        result.B = 0;
        if(R < 60.0)
        {
            result.R = C;
            result.G = X;
        }
        else
        {
            result.R = X;
            result.G = C;
        }
    }
    else if(R < 240)
    {
        result.R = 0;
        if(R < 180)
        {
            result.G  = C;
            result.B = X;
        }
        else
        {
            result.G  = X;
            result.B = C;
        }
    }
    else
    {
        result.G = 0;
        if(R < 300)
        {
            result.R = X;
            result.B = C;
        }
        else
        {
            result.R = C;
            result.B = X;
        }
    }
    return result;
}

Color Color::AddWithIntensivity(Color color, float a)
{
    Color c = color.RGBtoHSV();
    c.B = a;
    return *this + c.HSVtoRGB();
}

Color Color::operator +(Color& a)
{
    return Color(this->R + a.R, this->G + a.G, this->B + a.B);
}
Color Color::operator +(Color&& a)
{
    return Color(this->R + a.R, this->G + a.G, this->B + a.B);
}

Color Color::operator *(float a)
{
    return Color(this->R*a, this->G*a, this->B*a);
}
