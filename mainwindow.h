#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include "clickablelabel.h"
#include "line.h"
#include "map"
#include "editdrawabledialog.h"
#include "circle.h"
#include <memory>
QT_BEGIN_NAMESPACE

using std::string;
using std::map;
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    enum mode {line, circle, polygon, angle, rectangle, changing, border_fill};
    enum rectangle_changing{ nothing, up, right, down, left};
    ~MainWindow();

private slots:
    void label_clicked();
    void label_options();
    void label_radius();
    void label_edge_move();
    void label_move();
    void label_fill();
    void label_clip();
    void on_drawpolygon_clicked();
    void on_drawline_clicked();
    void on_drawcircle_clicked();
    void on_actionReset_triggered();
    void on_pushButton_clicked();

    void on_actionSave_triggered();

    void on_actionOpen_triggered();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_actionUsage_triggered();

    void on_borderfill_clicked();

private:
    Color border = Color(255, 255, 255);
    Color fill = Color(255, 255, 255);
    drawable* edited_shape;
    bool changingRadius = false;
    rectangle_changing rec_change = nothing;
    Color col = Color(0, 0, 0);
    mode _mode = line;
    bool from_start = true;
    drawable* current;
    std::vector<std::pair<Point* , drawable*>> points;
    string type;
    vector<drawable*> objects;
    Ui::MainWindow *ui;
    QImage _current;
    bool movingPolygon = false;
    bool movingRectangle = false;
    ClickableLabel* myLabel;
    Point* changingPoint = nullptr;
    EditDrawableDialog* dialog;
    void fillStartingFrom(Point p);
};

#endif // MAINWINDOW_H
