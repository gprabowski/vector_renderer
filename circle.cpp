#include "circle.h"

void circle::addPoint(Point* p) {
    if(points.size() == 0)
        points.push_back(p);
    else if(points.size() == 1){
        points.push_back(p);
        radius = distance(*points[0], *points[1]);
    }
    else {
        free(points[1]);
        points.pop_back();
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
    sh = cir;
    this->col = col;
}
