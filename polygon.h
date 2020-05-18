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
    void erase_clip(ClickableLabel* lab);
    bool isFinished() override;
    fillType getFilled();
    void setFilled(fillType val, Color col);
    int getYmax();
    int getYmin();
    void setYmax(int val) { ymax = val; };
    void setYmin(int val) { ymin = val; };
    void setImg(QImage i) { img = i; }
    void fill(ClickableLabel* myLabel, Color col = Color(255, 255, 255), bool clean = false);
    bool isConvex();
    void setClipper(bool val) {clipper = val;}
    void setClippedShape(polygon* p) {clippedShape = p;}
    void clip(ClickableLabel* lab);
    void setFilename(std::string f) { fileName = f; }
    std::string getFileName() { return fileName; }
    nlohmann::json Serialize() override;


private:
    QImage img;
    std::string fileName;
    bool clipper = false;
    polygon* clippedShape = nullptr;
    fillType filled = absent;
    bool finished;
    int ymin;
    int ymax;
    Color fillColor = Color(255, 255, 255);
};

#endif // POLYGON_H
