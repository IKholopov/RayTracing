#include "Photon.h"

Photon::Photon(Point position, Point direction):position_(position), direction_(direction) {}

Point Photon::Position() const
{
    return position_;
}

Point Photon::Direction() const
{
    return direction_;
}


bool Photon::IntersecWithBox(const Box& box, std::pair<Point, Point>& points) const
{
    //XY

    if(!IsFloatZero(direction_.X) || !IsFloatZero(direction_.Y))
    {
        if(IsFloatZero(direction_.X))
        {
            if(position_.X < box.XMin || position_.X > box.XMax)
                return false;
            points.first.X= position_.X;
            points.second.X= position_.X;
            points.first.Y = box.YMin;
            points.second.Y = box.YMax;
        }
        else
        {
            float k = direction_.Y / direction_.X;
            float b = position_.Y - position_.X * k;
            float xy_y_1 = k*box.XMin + b;
            float xy_y_2 = k*box.XMax + b;
            if((xy_y_1 < box.YMin && xy_y_2 < box.YMin) || (xy_y_1 > box.YMax && xy_y_2 > box.YMax))
                return false;
            bool lower = false;
            bool upper = false;
            if(xy_y_1 > box.YMin && xy_y_1 < box.YMax)
            {
                lower = true;
                points.first.X = box.XMin;
                points.first.Y = xy_y_1;
            }
            if(xy_y_2 > box.YMin && xy_y_2 < box.YMax)
            {
                upper = true;
                points.second.X = box.XMax;
                points.second.Y = xy_y_2;
            }
            if(lower && !upper)
            {
                if(k > 0)
                {
                    points.second.Y = box.YMax;
                    points.second.X = (box.YMax - b)/k;
                }
                else
                {
                    points.second.Y = box.YMin;
                    points.second.X = (box.YMin - b)/k;
                }
            }
            if(!lower && upper)
            {
                if(k > 0)
                {
                    points.first.Y = box.YMin;
                    points.first.X = (box.YMin - b)/k;
                }
                else
                {
                    points.first.Y = box.YMax;
                    points.first.X = (box.YMax - b)/k;
                }
            }
            if(!lower && !upper)
            {
                if(k > 0)
                {
                    points.first.Y = box.YMin;
                    points.first.X = (box.YMin - b)/k;
                    points.second.Y = box.YMax;
                    points.second.X = (box.YMax - b)/k;
                }
                else
                {
                    points.first.Y = box.YMax;
                    points.first.X = (box.YMax - b)/k;
                    points.second.Y = box.YMin;
                    points.second.X = (box.YMin - b)/k;
                }
            }
        }
    }
    else //XZ
    {
        if(position_.Z < box.ZMin || position_.Z > box.ZMax)
            return false;
        points.first.X = position_.X;
        points.second.X = position_.X;
        points.first.Y = position_.Y;
        points.second.Y = position_.Y;
        points.first.Z = box.ZMin;
        points.first.Z = box.ZMax;
    }
    if(IsFloatZero(direction_.X))
    {
        if(position_.X < box.XMin || position_.X > box.XMax)
            return false;
        points.first.Z = box.ZMin;
        points.second.Z = box.ZMax;
    }
    else
    {
        float k = direction_.Z / direction_.X;
        float b = position_.Z - position_.X * k;
        float z_1 = k*box.XMin + b;
        float z_2 = k*box.XMax + b;
        if((z_1 < box.ZMin && z_2 < box.ZMin) || (z_1 > box.ZMax && z_2 > box.ZMax))
            return false;
        bool lower = false;
        bool upper = false;
        if(z_1 > box.ZMin && z_1 < box.ZMax)
        {
            lower = true;
            points.first.X = box.XMin;
            points.first.Z = z_1;
        }
        if(z_2 > box.ZMin && z_2 < box.ZMax)
        {
            upper = true;
            points.second.X = box.XMax;
            points.second.Z = z_2;
        }
        if(lower && !upper)
        {
            if(k > 0)
            {
                points.second.Z = box.ZMax;
                points.second.X = (box.ZMax - b)/k;
            }
            else
            {
                points.second.Z = box.ZMin;
                points.second.X = (box.ZMin - b)/k;
            }
        }
        if(!lower && upper)
        {
            if(k > 0)
            {
                points.first.Z = box.ZMin;
                points.first.X = (box.ZMin - b)/k;
            }
            else
            {
                points.first.Z = box.ZMax;
                points.first.X = (box.ZMax - b)/k;
            }
        }
        if(!lower && !upper)
        {
            if(k > 0)
            {
                points.first.Z = box.ZMin;
                points.first.X = (box.ZMin - b)/k;
                points.second.Z = box.ZMax;
                points.second.X = (box.ZMax - b)/k;
            }
            else
            {
                points.first.Z = box.ZMax;
                points.first.X = (box.ZMax - b)/k;
                points.second.Z = box.ZMin;
                points.second.X = (box.ZMin - b)/k;
            }
        }
    }

    if((points.second - points.first)*direction_ < 0)
    {
        Point temp = points.first;
        points.first = points.second;
        points.second = temp;
    }
    return true;

}
