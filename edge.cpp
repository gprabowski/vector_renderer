#include "edge.h"



Edge::direction Edge::getDir() { return dir; }
void Edge::setDir(direction d) { dir = d; }
Edge::Edge(Point a, Point b) {
    int xmax = std::max(a.x, b.x);
    x = std::min(a.x, b.x);
    ymin = std::min(a.y, b.y);
    ymax = std::max(a.y, b.y);
    numerator = xmax - x;
    increment = denominator = ymax - ymin;
}
void Edge::makeStep() {
    if((increment += numerator) > denominator) {
        ++x;
        increment -= denominator;
    }
}

bool Edge::operator< (const Edge& edge) const{
    return x < edge.getX();
}



//GETTERS AND SETTERS
int Edge::getYmax() { return ymax; }
void Edge::setYmax(int y) { ymax = y; }
int Edge::getYmin() { return ymin; }
double Edge::getX() const { return x; }
void Edge::setX(double x) {this->x = x;}
int Edge::getNumerator() { return numerator; }
void Edge::setNumerator(int num) { numerator = num; }
void Edge::setDenominator(int num) { denominator = num; }
int Edge::getDenominator() { return denominator; }
