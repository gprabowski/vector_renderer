//midpoint line, brush, midpoint cicle (ver. 1), gupta-sproull
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clickablelabel.h"
#include <utility>
#include <QColorDialog>
#include <iostream>
#include "editdrawabledialog.h"
#include "polygon.h"
using std::cout;
using std::endl;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    myLabel = new ClickableLabel();
    connect(myLabel, &ClickableLabel::clicked, this, &MainWindow::label_clicked);
    connect(myLabel, &ClickableLabel::options, this, &MainWindow::label_options);
    myLabel->resize(500, 500);
    myLabel->setAlignment(Qt::AlignCenter);
    ui->verticalLayout_2->addWidget(myLabel);
    QPixmap* current = new QPixmap(500, 500);
    current->fill(Qt::white);
    _current = current->toImage();
    QPixmap* color_view = new QPixmap(ui->color_label->height(), ui->color_label->width());
    color_view->fill(QColor(col.r, col.g, col.b));
    ui->color_label->setPixmap(*color_view);
    myLabel->setPixmap(QPixmap::fromImage(_current));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::label_options() {
    bool close = false;
    Point c(myLabel->getX(), myLabel->getY());
    drawable *d;
    if(from_start) {
        for(auto p : points) {
            if(c.distance_small(*p.first)) {
                close = true;
                d = p.second;
                break;
            }
        }
        if(close) {
            dialog = new EditDrawableDialog(this, d->getColor(), d->getThickness());
            dialog->exec();
            if(!dialog->isCanceled()) {
                if(dialog->shouldDelete()) {
                    d->erase(myLabel);
                    unsigned long i;
                    vector<Point *> del = d->getPoints();
                    Point* first = del[0];
                    for(i = 0; i < points.size(); ++i)
                        if(points[i].first == first)
                            break;
                    if(del.size() == points.size()) points.clear();
                    else
                        points.erase(points.begin() + i, points.begin() + i + del.size() - 1);
                    cout << i << ' ' << del.size() << endl;
                    for(auto& point : del) free(point);

                } else {
                  d->setColor(dialog->getColor());
                  d->setThickness(dialog->getThickness());
                  d->draw(myLabel);
                }
            }
            delete dialog;
        }
    }
}
void MainWindow::label_clicked() {
    bool close = false;
    Point* c = new Point(myLabel->getX(), myLabel->getY());
    if(from_start) {
        for(auto p : points) {
            if(c->distance_small(*p.first)) {
                close = true;
                current = p.second;
                free(c);
                c = p.first;
                break;
            }
        }
        if(close) {
            from_start = false;
            changingPoint = c;
        } else {
            switch(_mode){
            case line:
                current = new class line(col);
                current->addPoint(c);
                points.push_back(std::make_pair(c, current));
                from_start = false;
                break;
            case circle:
                current = new class circle(col);
                current->addPoint(c);
                points.push_back(std::make_pair(c, current));
                from_start = false;
                break;
            case polygon:
                myLabel->setPixel(*c, col);
                current = new class polygon(col);
                current->addPoint(c);
                points.push_back(std::make_pair(c, current));
                from_start = false;
                break;
            }
        }
    }
    else{
        if(changingPoint != nullptr) {
            current->erase(myLabel);
            changingPoint->x = c->x;
            changingPoint->y = c->y;
            free(c);
            current->draw(myLabel);
            changingPoint = nullptr;
            from_start = true;
        } else {
            current->addPoint(c);
            if(!(_mode == circle))
                points.push_back(std::make_pair(c, current));
            if(current->isFinished()) {
                cout << "mode is: " << _mode << " evaluated finish" <<  endl;
                current->draw(myLabel);
                objects.push_back(current);
                current = nullptr;
                from_start = true;
            }
        }
    }
}


void MainWindow::on_drawline_clicked()
{
    cout << "mode changed to line" << endl;
    _mode = line;
    ui->drawline->setChecked(true);
    ui->drawcircle->setChecked(false);
    ui->drawpolygon->setChecked(false);
}


void MainWindow::on_drawcircle_clicked()
{
    cout << "mode changed to circle" << endl;
    _mode = circle;
    ui->drawline->setChecked(false);
    ui->drawcircle->setChecked(true);
    ui->drawpolygon->setChecked(false);

}

void MainWindow::on_drawpolygon_clicked()
{
    cout << "mode changed to polygon" << endl;
    _mode = polygon;
    ui->drawline->setChecked(false);
    ui->drawcircle->setChecked(false);
    ui->drawpolygon->setChecked(true);
}

void MainWindow::on_actionReset_triggered()
{
    from_start = true;
    changingPoint = nullptr;
    for(auto object : objects) {
        object->erase(myLabel);
        free(object);
    }
    objects.clear();
    for(int i = points.size() - 1; i >= 0; --i) {
        free(points[i].first);
        points.pop_back();
    }
}

void MainWindow::on_pushButton_clicked()
{
    QColor color = QColorDialog::getColor();
        if( color.isValid() )
        {
            QPixmap color_view(ui->color_label->height(), ui->color_label->width());
            color_view.fill(color);
            ui->color_label->setPixmap(color_view);
            int _r, _g, _b;
            color.getRgb(&_r, &_g, &_b);
            col = Color(_r, _g, _b);
        }
}
