#ifndef POLYGON_H
#define POLYGON_H
#include "drawable.h"

class polygon : public drawable
{
public:
    polygon();
    polygon(Color col);
    void addPoint(Point* p) override;
    Point* getPoint(int i) override;
    void draw(ClickableLabel* lab) override;
    void erase(ClickableLabel* lab) override;
    bool isFinished() override;
    bool isFilled();
    void setFilled(bool val);
    int getYmax();
    int getYmin();

private:
    bool finished;
    int ymin;
    int ymax;
    bool filled = false;
};

#endif // POLYGON_H
