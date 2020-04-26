#ifndef CIRCLE_H
#define CIRCLE_H
#include "drawable.h"
#include "utility_func.h"

class circle : public drawable
{
public:
    circle() {}
    circle(Color col);
    void addPoint(Point* p) override;
    Point* getPoint(int i) override;
    void draw(ClickableLabel* lab) override;
    void erase(ClickableLabel* lab) override;
    bool isFinished() override;

private:
    double radius;
};

#endif // CIRCLE_H
