#include "angle.h"

void angle::addPoint(Point* p) {
    if(points.size() == 0)
        points.push_back(p);
    else if(points.size() == 1){
        points.push_back(p);
        radius = distance(*points[0], *points[1]);
        angle_change = true;
    }
    else if(angle_change){
        angle_change = false;
        points.push_back(p);
    }
}
Point* angle::getPoint(int i) {
    return points[i];
}
void angle::draw(ClickableLabel* lab) {
    int x1 = points[0]->x, y1 = points[0]->y;
    drawMidpointAngle(x1, y1, points[1]->x, points[1]->y, points[2]->x, points[2]->y, radius, lab, col, false);
}
void angle::erase(ClickableLabel* lab) {
    int x1 = points[0]->x, y1 = points[0]->y;
    drawMidpointAngle(x1, y1, points[1]->x, points[1]->y, points[2]->x, points[2]->y, radius, lab, Color(255, 255, 255), false);

}
bool angle::isFinished() {
    return points.size() == 3;
}

angle::angle(Color col) {
    sh = ang;
    this->col = col;
}
