#ifndef UTILITY_FUNC_H
#define UTILITY_FUNC_H
#include "drawable.h"
#include "color.h"
void drawMidpointLine(int x1, int y1, int x2, int y2, Color col, ClickableLabel* lab, int n = 1);
void drawMidpointCircle(int x, int y, double r, ClickableLabel* lab, Color col, bool fill = false);
void drawMidpointAngle(int x, int y, int x2, int y2, int x3, int y3,  double r, ClickableLabel* lab, Color col, bool fill = false);
double distance(Point a, Point b);
void drawPositive(Point a, Point b, Point c, Point d, ClickableLabel * lab, Color col);
void drawNegative(Point a, Point b, Point c, Point d, ClickableLabel * lab, Color col);
Color getColorFromImg(int x, int y, QImage img);
#endif // UTILITY_FUNC_H
