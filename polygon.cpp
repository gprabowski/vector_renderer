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
