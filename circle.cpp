#include "circle.h"

void circle::addPoint(Point* p) {
    if(points.size() == 0)
        points.push_back(p);
    else {
        points.push_back(p);
        radius = distance(*points[0], *points[1]);
    }
}
Point* circle::getPoint(int i) {
    return points[i];
}
void circle::draw(ClickableLabel* lab) {
    int x1 = points[0]->x, y1 = points[0]->y;
    drawMidpointCircle(x1, y1, radius, lab, col, false);
}
void circle::erase(ClickableLabel* lab) {
    int x1 = points[0]->x, y1 = points[0]->y;
    drawMidpointCircle(x1, y1, radius, lab, Color(255, 255, 255));

}
bool circle::isFinished() {
    return points.size() == 2;
}

circle::circle(Color col) {
    this->col = col;
}
