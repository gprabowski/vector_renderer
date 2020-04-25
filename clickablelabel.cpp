#include "clickablelabel.h"
#include "QMouseEvent"
#include <iostream>

using std::cout;
using std::endl;
ClickableLabel::ClickableLabel(QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent) {

}

ClickableLabel::~ClickableLabel() {}

void ClickableLabel::mousePressEvent(QMouseEvent* event) {
    x = event->x() - (width() - 500) / 2.0;
    y = event->y() - (height() - 500) / 2.0;
    if(x >= 0 && x <= 499 && y >= 0 && y <= 499) {
        if (event->button() == Qt::LeftButton) {
            std::cout << "Left: " << x << ' ' << y << ' ' << std::endl;
            emit clicked();
        } else if(event->button() == Qt::RightButton) {
            std::cout << "Right: " << x << ' ' << y << ' ' << std::endl;
            emit options();
        }
    }
}
int ClickableLabel::getX() {return x;}
int ClickableLabel::getY() {return y;}

void ClickableLabel::setPixel(Point p, Color val) {
    if(p.x < 500 && p.x >= 0 && p.y < 500 && p.y >= 0) {
        auto image = this->pixmap()->toImage();
        auto b_image = image.bits();
        b_image[p.x * 4 + p.y * 500 * 4] = val.b;
        b_image[p.x * 4 + p.y * 500 * 4 + 1] = val.g;
        b_image[p.x * 4 + p.y * 500 * 4 + 2] = val.r;
        this->setPixmap(QPixmap::fromImage(image));
    }
}
