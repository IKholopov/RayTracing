#include <iostream>

#include "Photon.h"
#include "Base.h"

# define TEST(A, B) if(!A) std::cout << "Test " << B << " failed" << std::endl;

void TestPhotonBoxIntersect()
{
    Box box(5, 1 , 2, 0, 3, 0);
    std::pair<Point, Point> points;
    Photon photon(Point(0, 0, 0), Point(0, 1, 0));
    TEST(!photon.IntersecWithBox(box, points), "y line")
    std::cout << points.first << " " << points.second << std::endl;
    photon = Photon(Point(0, 0, 0), Point(5, 1, 2));
    TEST(photon.IntersecWithBox(box, points), "line")
    std::cout << points.first << " " << points.second << std::endl;
    box = Box(5, -5, 2, -2, 3, -3);
    photon = Photon(Point(0, 0, 0), Point(0, 1, 0));
    TEST(photon.IntersecWithBox(box, points), "y line")
    std::cout << points.first << " " << points.second << std::endl;
    photon = Photon(Point(0, 0, 0), Point(5, 1, 2));
    TEST(photon.IntersecWithBox(box, points), "line")
    std::cout << points.first << " " << points.second << std::endl;
}

int main( int argc, char **argv )
{
    TestPhotonBoxIntersect();
    return 0;
}
