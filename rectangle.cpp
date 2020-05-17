#include "rectangle.h"

rectangle::rectangle(Color col) {
    sh = rec;
    this->col = col;
}
void rectangle::addPoint(Point* p) {
    if(points.size() == 0)
        points.push_back(p);
    else {
        if(p->x > points[0]->x) {
            std::cout << "one " << std::endl;
            points.push_back(p);
        }
        else {
            std::cout << "two " << std::endl;
            points.push_back(p);
            std::swap(points[0], points[1]);
        }
        if(points[0]->y > points[1]->y) {
            std::cout << "three " << std::endl;

            std::swap(points[0]->y, points[1]->y);
        }

    }

}

Point* rectangle::getPoint(int i) {
    return points[i];
}

void rectangle::draw(ClickableLabel* lab) {
    drawMidpointLine(points[0]->x, points[0]->y, points[1]->x, points[0]->y, this->col, lab, this->thickness);
    drawMidpointLine(points[1]->x, points[1]->y, points[0]->x, points[1]->y, this->col, lab, this->thickness);
    drawMidpointLine(points[0]->x, points[0]->y, points[0]->x, points[1]->y, this->col, lab, this->thickness);
    drawMidpointLine(points[1]->x, points[1]->y, points[1]->x, points[0]->y, this->col, lab, this->thickness);
}

void rectangle::erase(ClickableLabel* lab) {
    drawMidpointLine(points[0]->x, points[0]->y, points[1]->x, points[0]->y, Color(255, 255, 255), lab, this->thickness);
    drawMidpointLine(points[1]->x, points[1]->y, points[0]->x, points[1]->y, Color(255, 255, 255), lab, this->thickness);
    drawMidpointLine(points[0]->x, points[0]->y, points[0]->x, points[1]->y, Color(255, 255, 255), lab, this->thickness);
    drawMidpointLine(points[1]->x, points[1]->y, points[1]->x, points[0]->y, Color(255, 255, 255), lab, this->thickness);
}

bool rectangle::isFinished() {
    return points.size() == 2;
}
