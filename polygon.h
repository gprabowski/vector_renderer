#ifndef POLYGON_H
#define POLYGON_H
#include "drawable.h"
#include "edge.h"
class polygon : public drawable
{
public:
    polygon();
    polygon(Color col);
    enum fillType{absent, solid, pattern};
    void addPoint(Point* p) override;
    Point* getPoint(int i) override;
    void draw(ClickableLabel* lab) override;
    void erase(ClickableLabel* lab) override;
    bool isFinished() override;
    bool isFilled();
    void setFilled(fillType val, Color col);
    int getYmax();
    int getYmin();
    void setYmax(int val) { ymax = val; };
    void setYmin(int val) { ymin = val; };
    void setImg(QImage i) { img = i; }
    void fill(Color col, ClickableLabel* myLabel);

private:
    QImage img;
    fillType filled = absent;
    bool finished;
    int ymin;
    int ymax;
    Color fillColor = Color(255, 255, 255);
};

#endif // POLYGON_H
