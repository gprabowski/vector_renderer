//midpoint line, brush, midpoint cicle (ver. 1), gupta-sproull
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clickablelabel.h"
#include <utility>
#include <QColorDialog>
#include <iostream>
#include "editdrawabledialog.h"
#include "polygon.h"
#include "drawable.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <QFileDialog>
#include "angle.h"
#include "rectangle.h"
#include "filldialog.h"
#include "edge.h"
#import <qmessagebox.h>

using std::cout;
using std::endl;
using json = nlohmann::json;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    myLabel = new ClickableLabel();
    connect(myLabel, &ClickableLabel::clicked, this, &MainWindow::label_clicked);
    connect(myLabel, &ClickableLabel::options, this, &MainWindow::label_options);
    connect(myLabel, &ClickableLabel::radius, this, &MainWindow::label_radius);
    connect(myLabel, &ClickableLabel::move, this, &MainWindow::label_move);
    connect(myLabel, &ClickableLabel::edge_move, this, &MainWindow::label_edge_move);
    connect(myLabel, &ClickableLabel::fill, this, &MainWindow::label_fill);
    connect(myLabel, &ClickableLabel::clip, this, &MainWindow::label_clip);


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
    myLabel->setup();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::label_clip() {
    bool close = false;
    Point c(myLabel->getX(), myLabel->getY());
    drawable *d;
    std::vector<Edge> AET;
    if(from_start) {
        for(auto p : points) {
            if(c.distance_small(*p.first) && p.second->getShape() == drawable::pol) {
                close = true;
                d = p.second;
                break;
            }
        }

        if(close) {
            if(!reinterpret_cast<class polygon* >(d)->isConvex()) {
                QMessageBox msgBox;
                msgBox.setWindowTitle("Usage");
                msgBox.setText("This polygon is <b>NOT</b> convex");
                msgBox.setInformativeText("\n\n and thus not suitable for clipping into");
                msgBox.setStandardButtons(QMessageBox::Close);
                msgBox.setDefaultButton(QMessageBox::Close);
                int ret = msgBox.exec();
            } else {
                from_start = false;
                edited_shape = d;
            }
        }
    } else {
        // not from start
        for(auto p : points) {
            if(c.distance_small(*p.first) && p.second->getShape() == drawable::pol) {
                close = true;
                d = p.second;
                break;
            }
        }
        if(close) {
            reinterpret_cast<class polygon*>(edited_shape)->setClippedShape(reinterpret_cast<class polygon*>(d));
            reinterpret_cast<class polygon*>(edited_shape)->setClipper(true);
            edited_shape->setColor(Color(255, 0, 0));
            d->setColor(Color(0, 255, 0));
            edited_shape->draw(myLabel);
            d->draw(myLabel);
        }
    }
}

void MainWindow::label_fill() {
    bool close = false;
    Point c(myLabel->getX(), myLabel->getY());
    drawable *d;
    std::vector<Edge> AET;
    if(from_start) {
        for(auto p : points) {
            if(c.distance_small(*p.first) && p.second->getShape() == drawable::pol) {
                close = true;
                d = p.second;
                break;
            }
        }
        if(close) {
            auto _dialog = new FillDialog(this, d->getColor());
            _dialog->exec();
            if(!_dialog->isCanceled()){
                if(_dialog->isSolid()) {
                    reinterpret_cast<class polygon*>(d)->setFilled(polygon::solid, _dialog->getColor());
                    reinterpret_cast<class polygon*>(d)->fill(_dialog->getColor(), myLabel);
                } else {
                    reinterpret_cast<class polygon*>(d)->setFilled(polygon::pattern, _dialog->getColor());
                    reinterpret_cast<class polygon*>(d)->setImg(QPixmap(_dialog->getFilename()).toImage());
                    reinterpret_cast<class polygon*>(d)->fill(_dialog->getColor(), myLabel);
                }
            }
        }
    }
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
                    auto it = objects.begin();
                    while(it != objects.end()) {
                        if(d == (*it))
                            break;
                        ++it;
                    }
                    objects.erase(it);
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
                drawMidpointCircle(c->x, c->y, 1, myLabel, col, true);
                current = new class polygon(col);
                current->addPoint(c);
                points.push_back(std::make_pair(c, current));
                from_start = false;
                break;
            case angle:
                myLabel->setPixel(*c, col);
                current = new class angle(col);
                current->addPoint(c);
                points.push_back(std::make_pair(c, current));
                from_start = false;
                break;
             case rectangle:
                myLabel->setPixel(*c, col);
                current = new class rectangle(col);
                current->addPoint(c);
                points.push_back(std::make_pair(c, current));
                from_start = false;
                break;
             case changing:
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

void MainWindow::label_radius() {
    Point c(myLabel->getX(), myLabel->getY());
    if(changingRadius == false)
        for(auto point : points) {
            if(point.second->getShape() != drawable::cir)
                continue;
            if(distance(c, *point.first) < 10) {
                cout << "changing" << endl;
                changingRadius = true;
                edited_shape = point.second;
                break;
            }
        }
    else {
        changingRadius = false;
        edited_shape->erase(myLabel);
        Point* d = new Point(myLabel->getX(), myLabel->getY());
        edited_shape->addPoint(d);
        edited_shape->draw(myLabel);
    }
}

void MainWindow::label_edge_move() {
    cout << "Executing edge move" << endl;
    Point c(myLabel->getX(), myLabel->getY());
    if(rec_change == nothing){
        for(auto point : points) {
            if(point.second->getShape() != drawable::rec)
                continue;
            //CHECK IF ITS UP
            if(abs(c.y - point.second[0].getPoint(0) -> y) < 10){
                if(c.x > point.second[0].getPoint(0)->x && c.x < point.second[0].getPoint(1)->x){
                    rec_change = up;
                    edited_shape = point.second;
                }
            }
            if(abs(c.x - point.second[0].getPoint(1) -> x) < 10){
                if(c.y > point.second[0].getPoint(0)->y && c.y < point.second[0].getPoint(1)->y){
                    rec_change = right;
                    edited_shape = point.second;
                }
            }
            if(abs(c.y - point.second[0].getPoint(1) -> y) < 10){
                if(c.x > point.second[0].getPoint(0)->x && c.x < point.second[0].getPoint(1)->x){
                    rec_change = down;
                    edited_shape = point.second;
                }
            }
            if(abs(c.x - point.second[0].getPoint(0) -> x) < 10){
                if(c.y > point.second[0].getPoint(0)->y && c.y < point.second[0].getPoint(1)->y){
                    rec_change = left;
                    edited_shape = point.second;
                }
            }

            }
        }
    else {
        switch(rec_change) {
        case up:
            cout << rec_change << endl;
            edited_shape->erase(myLabel);
            edited_shape[0].getPoint(0)->y = c.y;
            edited_shape->draw(myLabel);
            edited_shape = nullptr;
            rec_change = nothing;
            break;
        case right:
            cout << rec_change << endl;
            edited_shape->erase(myLabel);
            edited_shape[0].getPoint(1)->x = c.x;
            edited_shape->draw(myLabel);
            edited_shape = nullptr;
            rec_change = nothing;
            break;
        case down:
            cout << rec_change << endl;
            edited_shape->erase(myLabel);
            edited_shape[0].getPoint(1)->y = c.y;
            edited_shape->draw(myLabel);
            edited_shape = nullptr;
            rec_change = nothing;
            break;
        case left:
            cout << rec_change << endl;
            edited_shape->erase(myLabel);
            edited_shape[0].getPoint(0)->x = c.x;
            edited_shape->draw(myLabel);
            edited_shape = nullptr;
            rec_change = nothing;
            break;
        }
    }
}

void MainWindow::label_move() {
    Point c(myLabel->getX(), myLabel->getY());
    if(movingPolygon == false && movingRectangle == false)
        for(auto point : points) {
            if(point.second->getShape() != drawable::pol && point.second->getShape() != drawable::rec)
                continue;
            if(distance(c, *point.first) < 10) {
                if(point.second->getShape() == drawable::pol) {
                    cout << "movin pol" << endl;
                    movingPolygon = true;
                    edited_shape = point.second;
                    changingPoint = point.first;
                    break;
                }
                else {
                    cout << "movin rec" << endl;
                    movingRectangle = true;
                    edited_shape = point.second;
                    changingPoint = point.first;
                    break;
                }
            }
        }
    else {
        edited_shape->erase(myLabel);
        Point d(myLabel->getX(), myLabel->getY());
        int x_change = d.x - changingPoint->x;
        int y_change = d.y - changingPoint->y;
        if(movingPolygon) {
            movingPolygon = false;
            reinterpret_cast<class polygon*>(edited_shape)->setYmax(reinterpret_cast<class polygon*>(edited_shape)->getYmax() + y_change);
            reinterpret_cast<class polygon*>(edited_shape)->setYmin(reinterpret_cast<class polygon*>(edited_shape)->getYmin() + y_change);
            cout << "moving pol: " << x_change << ' ' << y_change << endl;
        }
        if(movingRectangle) {
            movingRectangle = false;
            cout << "moving rec: " << x_change << ' ' << y_change << endl;
        }
        for(auto point: edited_shape->getPoints()) {
            point->x += x_change;
            point->y += y_change;
        }
        edited_shape->draw(myLabel);
        changingPoint = nullptr;
        edited_shape = nullptr;
    }
}



void MainWindow::on_drawline_clicked()
{
    cout << "mode changed to line" << endl;
    _mode = line;
    ui->pushButton_2->setChecked(false);
    ui->drawline->setChecked(true);
    ui->drawcircle->setChecked(false);
    ui->drawpolygon->setChecked(false);
    ui->pushButton_3->setChecked(false);
    ui->pushButton_4->setChecked(false);


}


void MainWindow::on_drawcircle_clicked()
{
    cout << "mode changed to circle" << endl;
    _mode = circle;
    ui->pushButton_2->setChecked(false);
    ui->drawline->setChecked(false);
    ui->drawcircle->setChecked(true);
    ui->drawpolygon->setChecked(false);
    ui->pushButton_3->setChecked(false);
    ui->pushButton_4->setChecked(false);


}

void MainWindow::on_drawpolygon_clicked()
{
    cout << "mode changed to polygon" << endl;
    _mode = polygon;
    ui->pushButton_2->setChecked(false);
    ui->drawline->setChecked(false);
    ui->drawcircle->setChecked(false);
    ui->drawpolygon->setChecked(true);
    ui->pushButton_3->setChecked(false);
    ui->pushButton_4->setChecked(false);

}

void MainWindow::on_pushButton_3_clicked()
{
    cout << "mode changed to rectangle" << endl;
    _mode = rectangle;
    ui->pushButton_2->setChecked(false);
    ui->drawline->setChecked(false);
    ui->drawcircle->setChecked(false);
    ui->drawpolygon->setChecked(false);
    ui->pushButton_3->setChecked(true);
    ui->pushButton_4->setChecked(false);

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

void MainWindow::on_actionSave_triggered()
{
    auto fileName = QFileDialog::getSaveFileName(this,
            tr("Save Your Project"), "",
            tr("Project File(*.json);;All Files (*)"));
    json j;
    for(auto object : objects) {
        json a;
        a["shape"] = object->getShape();
        for(auto point : object->getPoints()) {
            a["points"].push_back({point->x, point->y});
        }
        a["color"] = {object->getColor().r, object->getColor().g, object->getColor().b};
        a["thickness"] = object->getThickness();
        a["size"] = object->getPoints().size();
        j.push_back(a);
    }
    std::ofstream file(fileName.toStdString());
        file << j;
}

void MainWindow::on_actionOpen_triggered()
{
    this->on_actionReset_triggered();
    auto fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "/home/", tr("Image Files (*.json)"));
    if(fileName.isEmpty())
        return;
    std::ifstream i(fileName.toStdString());
    json j;
    i >> j;
    cout << j.size() << endl;
    Color col2(0, 0, 0);
    int size;
    Point* p2;
    for (auto& element : j) {
      switch(element["shape"].get<int>()) {
      case 0:
          cout << "it's a circle" << endl;
          col2 = Color(element["color"][0], element["color"][1], element["color"][2]);
          current = new class circle(col2);
          size = element["size"].get<int>();
          for(auto point : element["points"]) {
              p2 = new Point(point[0].get<int>(), point[1].get<int>());
              current->addPoint(p2);
              points.push_back(std::make_pair(p2, current));
          }
          current->setColor(col2);
          current->setThickness(element["thickness"].get<int>());
          current->draw(myLabel);
          current = nullptr;
          break;
      case 1:
          cout << "it's a polygon" << endl;
          col2 = Color(element["color"][0], element["color"][1], element["color"][2]);
          current = new class polygon(col2);
          size = element["size"].get<int>();
          for(auto point : element["points"]) {
              p2 = new Point(point[0].get<int>(), point[1].get<int>());
              current->addPoint(p2);
              points.push_back(std::make_pair(p2, current));
          }
          current->setColor(col2);
          current->setThickness(element["thickness"].get<int>());
          current->draw(myLabel);
          current = nullptr;
          break;
      case 2:
          cout << "it's a line" << endl;
          col2 = Color(element["color"][0], element["color"][1], element["color"][2]);
          current = new class line(col2);
          size = element["size"].get<int>();
          for(auto point : element["points"]) {
              p2 = new Point(point[0].get<int>(), point[1].get<int>());
              current->addPoint(p2);
              points.push_back(std::make_pair(p2, current));
          }
          current->setColor(col2);
          current->setThickness(element["thickness"].get<int>());
          current->draw(myLabel);
          current = nullptr;
          break;
      }
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    cout << "mode changed to angle" << endl;
    _mode = angle;
    ui->drawline->setChecked(false);
    ui->drawcircle->setChecked(false);
    ui->drawpolygon->setChecked(false);
    ui->pushButton_3->setChecked(false);
    ui->pushButton_4->setChecked(false);
    ui->pushButton_2->setChecked(true);
    ui->pushButton->setChecked(false);

}


void MainWindow::on_pushButton_4_clicked()
{
    cout << "mode changed to changing" << endl;
    _mode = changing;
    ui->drawline->setChecked(false);
    ui->drawcircle->setChecked(false);
    ui->drawpolygon->setChecked(false);
    ui->pushButton_3->setChecked(false);
    ui->pushButton_2->setChecked(false);
    ui->pushButton_4->setChecked(true);
    ui->pushButton->setChecked(false);
}

void MainWindow::on_actionUsage_triggered()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Usage");
    msgBox.setText("<b>How to use this program.</b>");
    msgBox.setInformativeText("Shift + left click to move entire polygon/rectangle \n\n"
                              "Control + left click to change circle radius \n\n"
                              "Left click to move entire circle \n\n"
                              "Alt + left click to change Rectangle's side \n\n"
                              "Right click to change color/thickness \n\n"
                              "Ctr + right click to fill with texture/solid \n\n"
                              "Alt + right click to clip one polygon to another");
    msgBox.setStandardButtons(QMessageBox::Close);
    msgBox.setDefaultButton(QMessageBox::Close);
    int ret = msgBox.exec();
}
