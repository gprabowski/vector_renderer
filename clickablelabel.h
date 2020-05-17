#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>
#include <QWidget>
#include <Qt>
#include "point.h"
#include "color.h"
class ClickableLabel : public QLabel {
    Q_OBJECT

public:
    explicit ClickableLabel(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    int getX();
    int getY();
    void setup();
    void update();
    QImage getImage();
    void setPixel(Point p, Color val);
    ~ClickableLabel();

signals:
    void clicked();
    void options();
    void radius();
    void move();
    void fill();
    void edge_move();

protected:
    QImage image;
    uchar* b_img;
    int x;
    int y;
    void mousePressEvent(QMouseEvent* event);

};

#endif // CLICKABLELABEL_H
