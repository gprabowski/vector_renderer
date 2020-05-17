#ifndef EDGE_H
#define EDGE_H
#include "point.h"

class Edge
{
public:
    Edge();
    enum direction{right, left};
    Edge(Point a, Point b);
    int getYmax();
    void setYmax(int y);
    int getYmin();
    double getX() const;
    void setX(double x);
    direction getDir();
    void setDir(direction d);
    void makeStep();
    int getNumerator();
    void setNumerator(int num);
    void setDenominator(int num);
    int getDenominator();
    bool operator< (const Edge& edge) const;

private:
    int ymax;
    int ymin;
    double increment;
    double x;
    int numerator;
    int denominator;
    direction dir;
};

#endif // EDGE_H
