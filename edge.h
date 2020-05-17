#ifndef EDGE_H
#define EDGE_H
#include "point.h"

class Edge
{
public:
    Edge();
    Edge(const Edge& e);
    Edge(Edge&& e);
    ~Edge() {};
    enum direction{right, left};
    Edge(Point a, Point b);
    int getYmax() const;
    void setYmax(int y);
    int getYmin() const;
    double getX() const;
    void setX(double x);
    direction getDir() const;
    void setDir(direction d);
    void makeStep();
    int getNumerator() const;
    void setNumerator(int num);
    int getXmax() const;
    double getIncrement() const;
    void setDenominator(int num);
    int getDenominator() const;
    bool operator< (const Edge& edge) const;
    Edge& operator=(const Edge& e);

private:
    int ymax;
    int ymin;
    double increment;
    double x;
    int xmax;
    int numerator;
    int denominator;
    direction dir;
};

#endif // EDGE_H
