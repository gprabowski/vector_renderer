#include "point.h"

Point::Point() : x(0), y(0) {}
Point::Point(int x, int y) : x(x), y(y) {}
bool Point::operator<(const Point& p2) const {
    return (x + y) < (p2.x + p2.y);
}
bool Point::distance_small(const Point& p2) {
    return std::sqrt((x - p2.x)*(x - p2.x) + (y - p2.y) * (y - p2.y)) < 10;
}
Point Point::operator-(const Point& other) {
    return Point(x - other.x, y - other.y);
}

Point Point::operator*(const double& mul) {
    return Point(x * mul, y * mul);
}
