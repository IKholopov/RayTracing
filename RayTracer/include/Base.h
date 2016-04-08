#ifndef BASE_H
#define BASE_H

struct Resolution
{
        Resolution(unsigned int width, unsigned int height);
        unsigned int Width;
        unsigned int Height;
};

struct Color
{
        Color(float r, float g, float b);
        float R;
        float G;
        float B;
};

struct Point
{
        Point(float x, float y, float z);
        float X;
        float Y;
        float Z;

        Point operator -(Point& b);
        Point operator + (Point& a);
        Point operator * (float a);
        Point operator * (int a);
        Point operator * (unsigned int a);
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
        Box(Point x1y1z1, Point x1y1z2, Point x1y2z1, Point x1y2z2,
            Point x2y1z1, Point x2y1z2, Point x2y2z1,Point x2y2z2);

        Point x1y1z1;
        Point x1y1z2;
        Point x1y2z1;
        Point x1y2z2;
        Point x2y1z1;
        Point x2y1z2;
        Point x2y2z1;
        Point x2y2z2;
};

#endif
