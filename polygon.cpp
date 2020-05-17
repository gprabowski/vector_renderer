#include "polygon.h"
#include "utility_func.h"
#include <iostream>
#include "utility_func.h"


using std::cout;
using std::endl;
polygon::polygon()
{
    finished = false;
}
polygon::polygon(Color col){ this->col = col; finished = false; sh = pol;}

void polygon::addPoint(Point* p) {
    if(points.size() == 0){

        ymax = ymin = p->y;
    }
    else {
        ymax = std::max(ymax, p->y);
        ymin = std::min(ymin, p->y);
    }
    if(points.size() != 0)
        if(distance(*p, *points[0]) < 10) {
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
    drawMidpointCircle(points[0]->x, points[0]->y, 1, lab, Color(255, 255, 255), true);
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

bool polygon::isFilled() { return filled; }
void polygon::setFilled(bool val) { filled = val; }

int polygon::getYmax() { return ymax; }
int polygon::getYmin() { return ymin; }
