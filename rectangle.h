#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "drawable.h"
#include "clickablelabel.h"
#include "utility_func.h"

class rectangle : public drawable
{
public:
    rectangle() {this-> thickness = 1;}
    rectangle(Color col);
    void addPoint(Point* p) override;
    Point* getPoint(int i) override;
    void draw(ClickableLabel* lab) override;
    void erase(ClickableLabel* lab) override;
    bool isFinished() override;
};

#endif // RECTANGLE_H
