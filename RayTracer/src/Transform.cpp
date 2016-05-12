#include "Transform.h"

Transform::Transform(Point x, Point y, Point z, Point W):X(x), Y(y), Z(z), W(W)
{

}

Point Transform::Apply(Point a)
{
    Point result;
    result.X = a * X;
    result.Y = a * Y;
    result.Z = a * Z;
    result.X += W.X;
    result.Y += W.Y;
    result.Z += W.Z;
    return result;
}

Transform Transform::Move(Point move)
{
    Point X, Y, Z;
    X.X += 1;
    Y.Y += 1;
    Z.Z += 1;
    Point W = move;
    return Transform(X, Y, Z, W);
}
