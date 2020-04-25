#include "polygon.h"
#include "utility_func.h"
#include <iostream>

using std::cout;
using std::endl;
polygon::polygon()
{
    finished = false;
}
polygon::polygon(Color col){ this->col = col; finished = false;}

void polygon::addPoint(Point* p) {
    for(auto& point : points)
        if(distance(*p, *point) < 10) {
            cout << "changed to true" << endl;
            finished = true;
        }

    if(!finished)
        points.push_back(p);
}

Point* polygon::getPoint(int i) {
    return points[i];
}
int clamp(int i, int s) {
    return i%s;
}
void polygon::draw(ClickableLabel* lab) {
    for(unsigned long i = 0; i < points.size(); ++i) {
        drawMidpointLine(points[clamp(i, points.size())]->x,
                points[clamp(i, points.size())]->y,
                points[clamp(i + 1, points.size())]->x,
                points[clamp(i + 1, points.size())]->y,
                col, lab, thickness);
    }
}

void polygon::erase(ClickableLabel* lab) {
    for(unsigned long i = 0; i < points.size(); ++i) {
        drawMidpointLine(points[clamp(i, points.size())]->x,
                points[clamp(i, points.size())]->y,
                points[clamp(i + 1, points.size())]->x,
                points[clamp(i + 1, points.size())]->y,
                Color(255, 255, 255), lab, thickness);
    }
}

bool polygon::isFinished() { return finished; }
