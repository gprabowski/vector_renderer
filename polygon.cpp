#include "polygon.h"
#include "utility_func.h"
#include <iostream>
#include "utility_func.h"
#include "filldialog.h"
#include "edge.h"

using std::cout;
using std::endl;
polygon::polygon()
{
    finished = false;
}
polygon::polygon(Color col){ this->col = col; finished = false; sh = pol; }

void polygon::addPoint(Point* p) {
    if(points.size() == 0){
        ymax = ymin = p->y;
    }
    else {
        if(distance(*p, *points[0]) < 10) {
            cout << "changed to true" << endl;
            finished = true;
        } else {
            ymax = std::max(ymax, p->y);
            ymin = std::min(ymin, p->y);
        }
    }

    if(!finished)
        points.push_back(p);
}

Point* polygon::getPoint(int i) {
    return points[i];
}
int clamp(int i, int s) {
    return i%s;
}
void polygon::draw(ClickableLabel* lab) {
    drawMidpointCircle(points[0]->x, points[0]->y, 1, lab, Color(255, 255, 255), true);
    for(unsigned long i = 0; i < points.size(); ++i) {
        drawMidpointLine(points[clamp(i, points.size())]->x,
                points[clamp(i, points.size())]->y,
                points[clamp(i + 1, points.size())]->x,
                points[clamp(i + 1, points.size())]->y,
                col, lab, thickness);
    }
    if(filled == solid)
        fill(fillColor, lab);
    else if(clipper) {
        erase_clip(lab);
        clip(lab);
    }
}

void polygon::erase(ClickableLabel* lab) {
    for(unsigned long i = 0; i < points.size(); ++i) {
        drawMidpointLine(points[clamp(i, points.size())]->x,
                points[clamp(i, points.size())]->y,
                points[clamp(i + 1, points.size())]->x,
                points[clamp(i + 1, points.size())]->y,
                Color(255, 255, 255), lab, thickness);
    }
    if(filled == solid || filled == pattern) {
        filled = solid;
        fill(Color(255, 255, 255), lab);
    }
}

bool polygon::isFinished() { return finished; }

bool polygon::isFilled() { return filled; }
void polygon::setFilled(fillType val, Color col) { filled = val; fillColor = col; }

int polygon::getYmax() { return ymax; }
int polygon::getYmin() { return ymin; }

void polygon::fill(Color col, ClickableLabel* myLabel) {
        std::vector<std::vector<Edge>> ET(ymax - ymin);
        std::vector<Edge> AET;
        int w, h;
        if(filled == pattern) {
            w = img.width();
            h = img.height();
        }
        //FILLING ET WILL ALL EDGES OF THIS POLYGON
        for(int i = 0; i < (int)points.size(); ++i){
            Edge e(*(points[i]), *(points[(i+1) % points.size()]));
            //skipping horizontal
            if(e.getYmax() == e.getYmin())
                continue;
            cout << "putting an edge" << e.getYmax() << ' ' << e.getYmin() << ' ' << e.getX() << ' ' << e.getXmax() <<" into: " << ymax - e.getYmax() << "which now is of size" <<
                    ET[ymax - e.getYmax()].size() << endl;
            ET[ymax - e.getYmax()].push_back(e);

        }

        for(int i = 0; i < (int)ET.size(); ++i) {
            int level = ymax - i;
            //moving all those for which ymin = i
            for(auto& elem : ET[i]){
                AET.push_back(elem);
            }

            auto it = AET.begin();
            while(it != AET.end()){
                if(it->getYmin() == level) {
                    cout << "erasing" << endl;
                    it = AET.erase(it);
                } else {
                    ++it;
                }
            }
            std::sort(AET.begin(), AET.end());
            cout << "the new size of AET is " << AET.size() << endl;
            //for each pair fill the space between them
            for(int j = 0; j < (int)AET.size(); j += 2) {
                cout << "SIZE " << AET[j + 1].getX() << " " << AET[j].getX() << endl;
                for(int k = AET[j].getX(); k <= AET[j + 1].getX(); ++k) {
                    if(filled == solid) myLabel->setPixel(Point(k, level), col);
                    if(filled == pattern) myLabel->setPixel(Point(k, level), Color(img.bits()[(k%w)*4 + (level%h)*w*4 + 2],
                                                                                    img.bits()[(k%w)*4 + (level%h)*w*4 + 1],
                                                                                    img.bits()[(k%w)*4 + (level%h)*w*4]));
                }

            }

            for(auto& e : AET) {
                e.makeStep();
            }
            std::sort(AET.begin(), AET.end());
        }
            myLabel->update();
}

bool polygon::isConvex() {
    bool pos = false, neg = false;
    for(int i = 0; i < (int)points.size(); ++i) {
        if(cross_product(*points[i], *points[(i + 1) % points.size()], *points[(i + 2) % points.size()]) > 0)
            pos = true;
        else if(cross_product(*points[i], *points[(i + 1) % points.size()], *points[(i + 2) % points.size()]) < 0)
            neg = true;
        if(pos&&neg) return false;
    }
    return true;
}

double dot_product(Point a, Point b) {
    return a.x * b.x + a.y * b.y;
}

inline Point parametrized_equation(double t, Point a, Point b) {
    return Point(a - (a - b) * t);
}

void polygon::erase_clip(ClickableLabel* lab) {
    if(clipper) {
        clippedShape->draw(lab);
        this->clip(lab);
    }
}

void polygon::clip(ClickableLabel* lab){
    if(!isConvex()) {
        clipper = false;
        clippedShape->erase(lab);
        clippedShape->setThickness(1);
        clippedShape->draw(lab);
        return;
    }
    clippedShape->setThickness(3);
    clippedShape->draw(lab);
    bool clockwise = false;
    int h = points[0]->y;
    int pos = 0;
    for(int i = 1; i < (int)points.size(); ++i)
        if(points[i]->y < h) {
            pos = i;
            h = points[i]->y;
        }
//    if(points[pos]->x > points[(pos + 1)%points.size()]->x || points[pos]->x < points[(pos - 1) % points.size()]->x)
//        clockwise = false;
//    clockwise ? cout << "CLOCKWISE" << endl : cout << "COUNTER - CLOCKWISE" << endl;

    for(int i = 0; i < (int)clippedShape->getPoints().size(); ++i) {
        double te = 0, tl = 1;
        Point* p0 = clippedShape->getPoint(i);
        Point* p1 = clippedShape->getPoint((i + 1) % clippedShape->getPoints().size());
        Point d = *p1 - *p0;
//        if(p0->x > p1->x)
//            std::swap(p0, p1);
        cout << "POINTS  " << p0->x << ' ' << p1->x << endl;
        for(int j = 0; j < (int)points.size(); ++j) {
            //classify and find internal intersections
            Point* a = points[j % points.size()];
            Point* b = points[(j + 1) % points.size()];
            Point pe((a->x + b->x)/2, (a->y + b->y)/2);
            Point n_vec(b->y - a->y, b->x - a->x);
            if(clockwise) n_vec.x = (-1) * n_vec.x;
            else n_vec.y = (-1) * n_vec.y;
            double dp = dot_product(Point(n_vec), d);
            double t = dot_product(n_vec, *p0 - pe) / dot_product(n_vec * -1, d);
            if(dp < 0) te = std::max(t, te);
            else if(dp > 0) tl = std::min(t, tl);
            else continue;
        }
        if(te > tl) continue;
        drawMidpointLine(parametrized_equation(te, *p0, *p1).x, parametrized_equation(te, *p0, *p1).y,
                         parametrized_equation(tl, *p0, *p1).x, parametrized_equation(tl, *p0, *p1).y,
                         Color(0, 255, 0), lab, 3);

    }
}

nlohmann::json polygon::Serialize() {
    nlohmann::json a;
    a["shape"] = this->getShape();
    for(auto point : this->getPoints()) {
        a["points"].push_back({point->x, point->y});
    }
    a["color"] = {this->getColor().r, this->getColor().g, this->getColor().b};
    a["filled"] = filled;
    if(filled == solid)
        a["fill"] = {fillColor.r, fillColor.g, fillColor.b};
    else if(filled == pattern)
        a["fill"] = fileName;
    img.save(QString::fromStdString(fileName));
    a["thickness"] = this->getThickness();
    a["size"] = this->getPoints().size();
    cout << "saved a polygon!!!" << endl;
    return a;
}
