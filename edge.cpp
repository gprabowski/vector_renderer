#include "edge.h"



Edge::direction Edge::getDir() const{ return dir; }
void Edge::setDir(direction d) { dir = d; }
Edge::Edge(Point a, Point b) {
    xmax = std::fmax(a.x, b.x);
    int xmin = std::fmin(a.x, b.x);
    if(a.y < b.y)
        x = b.x;
    else
        x = a.x;
    ymin = std::fmin(a.y, b.y);
    ymax = std::fmax(a.y, b.y);
    if(x == xmin) dir = right;
    else dir = left;
    numerator = xmax - xmin;
    increment = denominator = ymax - ymin;
}
void Edge::makeStep() {
    increment += numerator;
    while(increment > denominator) {
        dir == right ? ++x : --x;
        increment -= denominator;
    }
}

bool Edge::operator< (const Edge& edge) const{
    return x < edge.getX();
}

Edge& Edge::operator=(const Edge& e) {
    ymax = e.getYmax();
    ymin = e.getYmin();
    increment = e.getIncrement();
    x = e.getX();
    xmax = e.getXmax();
    numerator = e.getNumerator();
    denominator = e.getDenominator();
    dir = e.getDir();
    return *this;
}

Edge::Edge(const Edge& e) {
    this->ymax = e.getYmax();
    this->ymin = e.getYmin();
    this->increment = e.getIncrement();
    this->x = e.getX();
    this->xmax = e.getXmax();
    this->numerator = e.getNumerator();
    this->denominator = e.getDenominator();
    this->dir = e.getDir();
}

//GETTERS AND SETTERS
Edge::Edge(Edge&& e){
    this->ymax = e.getYmax();
    this->ymin = e.getYmin();
    this->increment = e.getIncrement();
    this->x = e.getX();
    this->xmax = e.getXmax();
    this->numerator = e.getNumerator();
    this->denominator = e.getDenominator();
    this->dir = e.getDir();
}

int Edge::getXmax() const{ return xmax; }
int Edge::getYmax() const { return ymax; }
void Edge::setYmax(int y) { ymax = y; }
int Edge::getYmin() const{ return ymin; }
double Edge::getX() const { return this->x; }
void Edge::setX(double x) {this->x = x;}
double Edge::getIncrement() const {return increment; }
int Edge::getNumerator() const { return numerator; }
void Edge::setNumerator(int num) { numerator = num; }
void Edge::setDenominator(int num) { denominator = num; }
int Edge::getDenominator() const { return denominator; }
