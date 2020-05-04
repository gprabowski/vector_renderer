#include "utility_func.h"


void drawMidpointLine(int x1, int y1, int x2, int y2, Color col, ClickableLabel* lab, int n) {
    const bool steep = (abs(y2 - y1) > abs(x2 - x1));
    if(steep)
    {
      std::swap(x1, y1);
      std::swap(x2, y2);
    }

    if(x1 > x2){
        std::swap(x1, x2);
        std::swap(y1, y2);
    }
    int dx = x2 - x1;
    int dy = abs(y2 - y1);
    int d = 2*dy - dx;
    int dE = 2*dy;
    int dNE = 2*(dy - dx);
    int x = x1, y = y1;
    const int ystep = (y1 < y2) ? 1 : -1;
    steep ? drawMidpointCircle(y1, x1, (n - 1) / 2, lab, col, true) : drawMidpointCircle(x1, y1, (n - 1) / 2, lab, col, true);
    while(x < x2) {
        if ( d < 0 ) {
            d += dE;
            ++x;
        }
        else {
            d += dNE;
            ++x;
            y += ystep;
        }
        if(!steep)
            drawMidpointCircle(x, y, (n - 1) / 2, lab, col, true);
        else
            drawMidpointCircle(y, x, (n - 1) / 2, lab, col, true);
    }
    lab->update();
}

void drawMidpointCircle(int x, int y, double r, ClickableLabel* lab, Color col, bool fill) {
    int d = 1 - r;
    int _x = 0;
    int _y = r;
    if(fill) {
        for(int i  = x - r; i < x + r; ++i) {
            lab->setPixel(Point(i, y), col);
        }
        for(int i = y - r; i < y + r; ++i) {
            lab->setPixel(Point(x, i), col);
        }
    }
    lab->setPixel(Point(_x + x, _y + y), col);
    if(r > 0) {
        lab->setPixel(Point(x -_x, y + _y), col);
        lab->setPixel(Point(x + _x, y - _y), col);
        lab->setPixel(Point(x - _x, y - _y), col);
        lab->setPixel(Point(_y + x, _x + y), col);
        lab->setPixel(Point(x - _y, _x + y), col);
        lab->setPixel(Point(_y + x, y - _x), col);
        lab->setPixel(Point(x - _y, y - _x), col);
    }
    while(_y > _x) {
        if(d < 0)
            d+= 2*_x + 3;
        else
        {
            d += 2*_x - 2*_y + 5;
            --_y;
        }
        ++_x;
        lab->setPixel(Point(x + _x, y + _y), col);
        lab->setPixel(Point(x -_x, y + _y), col);
        lab->setPixel(Point(x + _x, y - _y), col);
        lab->setPixel(Point(x - _x, y - _y), col);
        lab->setPixel(Point(_y + x, _x + y), col);
        lab->setPixel(Point(x - _y, _x + y), col);
        lab->setPixel(Point(_y + x, y - _x), col);
        lab->setPixel(Point(x - _y, y - _x), col);

        if(fill) {
            for(int i  = 0; i < _y; ++i) {
                lab->setPixel(Point(x + _x, y + i), col);
                lab->setPixel(Point(x + _x, y - i), col);
                lab->setPixel(Point(x - _x, y + i), col);
                lab->setPixel(Point(x - _x, y - i), col);
                lab->setPixel(Point(x + i, y + _x), col);
                lab->setPixel(Point(x - i, y + _x), col);
                lab->setPixel(Point(x + i, y - _x), col);
                lab->setPixel(Point(x - i, y - _x), col);

            }
        }
    }
    lab->update();
}

double determinant(Point a, Point b, Point c) {
    return a.x*b.y - a.x*c.y - a.y*b.x + a.y*c.x + b.x*c.y - b.y*c.x;
}

void drawPositive(Point a, Point b, Point c, Point d, ClickableLabel * lab, Color col) {
    if(determinant(a, b, d) > 0 && determinant(a, c,  d) < 0)
        lab->setPixel(d, col);
}

void drawNegative(Point a, Point b, Point c, Point d, ClickableLabel * lab, Color col) {
    if(determinant(a, b, d) > 0 || determinant(a, c, d) < 0)
            lab->setPixel(d, col);
}


void drawMidpointAngle(int x, int y, int x2, int y2, int x3, int y3,  double r, ClickableLabel* lab, Color col, bool fill) {
    int d = 1 - r;
    int _x = 0;
    int _y = r;
    Point a(x, y);
    Point b(x2, y2);
    Point c(x3, y3);
    double det = determinant(Point(x,y), Point(x2, y2), Point(x3, y3));
    bool case1 = det >= 0;
    if(fill) {
        for(int i  = x - r; i < x + r; ++i) {
            lab->setPixel(Point(i, y), col);
        }
        for(int i = y - r; i < y + r; ++i) {
            lab->setPixel(Point(x, i), col);
        }
    }
    if(case1) {
        drawPositive(a, b, c, Point(_x + x, _y + y), lab, col);
        if(r > 0) {
            drawPositive(a, b, c, Point(x -_x, y + _y), lab, col);
            drawPositive(a, b, c, Point(x + _x, y - _y), lab, col);
            drawPositive(a, b, c, Point(x - _x, y - _y), lab, col);
            drawPositive(a, b, c, Point(_y + x, _x + y), lab, col);
            drawPositive(a, b, c, Point(x - _y, _x + y), lab, col);
            drawPositive(a, b, c, Point(_y + x, y - _x), lab, col);
            drawPositive(a, b, c, Point(x - _y, y - _x), lab, col);
        }
    }
    else if(!case1) {
        drawNegative(a, b, c, Point(_x + x, _y + y), lab, col);
        if(r > 0) {
            drawNegative(a, b, c, Point(x -_x, y + _y), lab, col);
            drawNegative(a, b, c, Point(x + _x, y - _y), lab, col);
            drawNegative(a, b, c, Point(x - _x, y - _y), lab, col);
            drawNegative(a, b, c, Point(_y + x, _x + y), lab, col);
            drawNegative(a, b, c, Point(x - _y, _x + y), lab, col);
            drawNegative(a, b, c, Point(_y + x, y - _x), lab, col);
            drawNegative(a, b, c, Point(x - _y, y - _x), lab, col);
        }
    }

    while(_y > _x) {
        if(d < 0)
            d+= 2*_x + 3;
        else
        {
            d += 2*_x - 2*_y + 5;
            --_y;
        }
        ++_x;
        if(case1) {
            drawPositive(a, b, c, Point(_x + x, _y + y), lab, col);
            drawPositive(a, b, c, Point(x -_x, y + _y), lab, col);
            drawPositive(a, b, c, Point(x + _x, y - _y), lab, col);
            drawPositive(a, b, c, Point(x - _x, y - _y), lab, col);
            drawPositive(a, b, c, Point(_y + x, _x + y), lab, col);
            drawPositive(a, b, c, Point(x - _y, _x + y), lab, col);
            drawPositive(a, b, c, Point(_y + x, y - _x), lab, col);
            drawPositive(a, b, c, Point(x - _y, y - _x), lab, col);

        }
        else if(!case1) {
            drawNegative(a, b, c, Point(_x + x, _y + y), lab, col);
            drawNegative(a, b, c, Point(x -_x, y + _y), lab, col);
            drawNegative(a, b, c, Point(x + _x, y - _y), lab, col);
            drawNegative(a, b, c, Point(x - _x, y - _y), lab, col);
            drawNegative(a, b, c, Point(_y + x, _x + y), lab, col);
            drawNegative(a, b, c, Point(x - _y, _x + y), lab, col);
            drawNegative(a, b, c, Point(_y + x, y - _x), lab, col);
            drawNegative(a, b, c, Point(x - _y, y - _x), lab, col);

        }

        if(fill) {
            for(int i  = 0; i < _y; ++i) {
                lab->setPixel(Point(x + _x, y + i), col);
                lab->setPixel(Point(x + _x, y - i), col);
                lab->setPixel(Point(x - _x, y + i), col);
                lab->setPixel(Point(x - _x, y - i), col);
                lab->setPixel(Point(x + i, y + _x), col);
                lab->setPixel(Point(x - i, y + _x), col);
                lab->setPixel(Point(x + i, y - _x), col);
                lab->setPixel(Point(x - i, y - _x), col);

            }
        }
    }
    drawMidpointLine(a.x, a.y, b.x, b.y, col, lab);
    auto dx = c.x - a.x;
    auto dy = c.y - a.y;
    dx = (dx/distance(a, c))*r + a.x;
    dy = (dy/distance(a, c))*r + a.y;
    drawMidpointLine(a.x, a.y, dx, dy, col, lab);
}

double distance(Point a, Point b) {
    return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}
