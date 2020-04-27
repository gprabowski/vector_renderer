#ifndef ANGLE_H
#define ANGLE_H

#include "drawable.h"
class angle : public drawable
{
public:
    angle() {}
    angle(Color col);
    void addPoint(Point* p) override;
    Point* getPoint(int i) override;
    void draw(ClickableLabel* lab) override;
    void erase(ClickableLabel* lab) override;
    bool isFinished() override;

private:
    double radius;
    bool angle_change = false;
};

#endif // ANGLE_H
