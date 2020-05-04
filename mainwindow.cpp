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
            case angle:
                myLabel->setPixel(*c, col);
                current = new class angle(col);
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

void MainWindow::label_move() {
    Point c(myLabel->getX(), myLabel->getY());
    if(movingPolygon == false)
        for(auto point : points) {
            if(point.second->getShape() != drawable::pol)
                continue;
            if(distance(c, *point.first) < 10) {
                cout << "movin pol" << endl;
                movingPolygon = true;
                edited_shape = point.second;
                changingPoint = point.first;
                break;
            }
        }
    else {
        movingPolygon = false;
        edited_shape->erase(myLabel);
        Point d(myLabel->getX(), myLabel->getY());
        int x_change = d.x - changingPoint->x;
        int y_change = d.y - changingPoint->y;
        cout << "moving pol: " << x_change << ' ' << y_change << endl;
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
}


void MainWindow::on_drawcircle_clicked()
{
    cout << "mode changed to circle" << endl;
    _mode = circle;
    ui->pushButton_2->setChecked(false);
    ui->drawline->setChecked(false);
    ui->drawcircle->setChecked(true);
    ui->drawpolygon->setChecked(false);

}

void MainWindow::on_drawpolygon_clicked()
{
    cout << "mode changed to polygon" << endl;
    _mode = polygon;
    ui->pushButton_2->setChecked(false);
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
    ui->pushButton->setChecked(true);
}
