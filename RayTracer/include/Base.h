#ifndef BASE_H
#define BASE_H

#include <ostream>
#include <istream>
#include <cmath>

enum Axis{ A_X = 0, A_Y = 1, A_Z = 2};

struct Resolution
{
        Resolution(unsigned int width, unsigned int height);
        unsigned int Width;
        unsigned int Height;
};

struct Color
{
        Color();
        Color(float r, float g, float b);
        float R;
        float G;
        float B;
        Color RGBtoHSV();
        Color HSVtoRGB();
        Color AddWithIntensivity(Color color, float a);
        Color operator + (Color& a);
        Color operator + (Color&& a);
        Color operator * (float a);
        friend std::ostream& operator<<( std::ostream& output, const Color& p)
        {
            output << p.R << " " << p.G << " " << p.B;
            return output;
        }
};

struct Point
{
        Point();
        Point(float x, float y, float z);
        float X;
        float Y;
        float Z;
        float GetAxis(Axis axis);

        float Length();
        Point Normalized();

        Point operator -(Point& b);
        Point operator -(Point&& b);
        Point operator + (Point& a);
        Point operator + (Point&& a);
        Point operator * (float a);
        Point operator * (double a);
        Point operator * (int a);
        Point operator * (unsigned int a);
        float operator * (const Point& b);
        float operator * (Point&& b);
        Point operator ^(const Point& b);
        Point operator ^ (Point&& b);

        friend std::ostream& operator<<( std::ostream& output, const Point& p)
        {
            output << p.X << " " << p.Y << " " << p.Z;
            return output;
        }
        friend std::istream& operator>>( std::istream& input, Point& p)
        {
            input >> p.X >> p.Y >> p.Z;
            return input;
        }
};

struct Plane
{
        Plane(Point x1y1z1, Point x2y2z2, Point x3y3z3, Point x4y4z4);
        Point x1y1z1;
        Point x2y2z2;
        Point x3y3z3;
        Point x4y4z4;
};

class Box
{
    public:
        Box();
        Box(float xMax, float xMin, float yMax, float yMin, float zMax, float zMin);

        float XMax;
        float XMin;
        float YMax;
        float YMin;
        float ZMax;
        float ZMin;

        float XLength();
        float YLength();
        float ZLength();
        float GetMid(Axis axis) const;
        float GetMax(Axis axis) const;
        float GetMin(Axis axis) const;
        void SetAxisMin(Axis axis, float val);
        void SetAxisMax(Axis axis, float val);
        bool IsInside(Point point);
};

bool IsFloatZero(float a);

#endif
