#include "line.h"

void line::addPoint(Point* p) {
    points.push_back(p);
}
Point* line::getPoint(int i) {
    return points[i];
}
void line::draw(ClickableLabel* lab) {
    int x1 = points[0]->x, y1 = points[0]->y, x2 = points[1]->x, y2 = points[1]->y;
    drawMidpointLine(x1, y1, x2, y2, col, lab, thickness);
}
void line::erase(ClickableLabel* lab) {
    int x1 = points[0]->x, y1 = points[0]->y, x2 = points[1]->x, y2 = points[1]->y;
    drawMidpointLine(x1, y1, x2, y2, Color(255, 255, 255), lab, thickness);
}

bool line::isFinished() {
    return points.size() == 2;
}

line::line(Color col) {
    sh = lin;
    this->col = col;
}
