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
        if(event->modifiers() == Qt::ControlModifier && event->button() == Qt::RightButton) {
            std::cout << "Fill click: " << x << ' ' << y << ' ' << std::endl;
            emit fill();
        }
        else if(event->modifiers() == Qt::AltModifier && event->button() == Qt::RightButton) {
            std::cout << "Clip click: " << x << ' ' << y << ' ' << std::endl;
            emit clip();
        }
        else if(event->button() == Qt::RightButton) {
            std::cout << "Right: " << x << ' ' << y << ' ' << std::endl;
            emit options();
        }
        else if(event->modifiers() == Qt::ControlModifier && event->button() == Qt::LeftButton) {
                    std::cout << "Radius click: " << x << ' ' << y << ' ' << std::endl;
                    emit radius();
                }
        else if(event->modifiers() == Qt::ShiftModifier && event->button() == Qt::LeftButton) {
                    std::cout << "Move click: " << x << ' ' << y << ' ' << event->modifiers() << std::endl;
                    emit move();
                }
        else if(event->modifiers() == Qt::AltModifier && event->button() == Qt::LeftButton) {
                    std::cout << "Rectangle Edge Click: " << x << ' ' << y << ' ' << event->modifiers() << std::endl;
                    emit edge_move();
                }
        else if (event->button() == Qt::LeftButton) {
            std::cout << "Left: " << x << ' ' << y << ' ' << std::endl;
            emit clicked();
        }
    }
}
int ClickableLabel::getX() {return x;}
int ClickableLabel::getY() {return y;}
QImage ClickableLabel::getImage() { return image; }
void ClickableLabel::setPixel(Point p, Color val) {
    if(p.x < 500 && p.x >= 0 && p.y < 500 && p.y >= 0) {
        b_img[p.x * 4 + p.y * 500 * 4] = val.b;
        b_img[p.x * 4 + p.y * 500 * 4 + 1] = val.g;
        b_img[p.x * 4 + p.y * 500 * 4 + 2] = val.r;
    }
}

Color ClickableLabel::getPixel(Point p) {
    if(p.x < 500 && p.x >= 0 && p.y < 500 && p.y >= 0) {
        return Color(b_img[p.x * 4 + p.y * 500 * 4 + 2],
        b_img[p.x * 4 + p.y * 500 * 4 + 1],
        b_img[p.x * 4 + p.y * 500 * 4]);
    }
    else return Color(255, 255, 255);
}

void ClickableLabel::setup() {
    image = this->pixmap()->toImage();
    b_img = image.bits();
}

void ClickableLabel::update() {
    this->setPixmap(QPixmap::fromImage(image));
}
