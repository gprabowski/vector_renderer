#ifndef POINT_H
#define POINT_H
#include <cmath>
#include <iostream>
struct Point {
    int x;
    int y;
    Point();
    Point(int x, int y);
    bool operator<(const Point& p2) const;
    bool distance_small(const Point& p2);

};
#endif // POINT_H
