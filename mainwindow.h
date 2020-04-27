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
    enum mode {line, circle, polygon, angle};
    ~MainWindow();

private slots:
    void label_clicked();
    void label_options();
    void label_radius();
    void label_move();
    void on_drawpolygon_clicked();
    void on_drawline_clicked();
    void on_drawcircle_clicked();
    void on_actionReset_triggered();
    void on_pushButton_clicked();

    void on_actionSave_triggered();

    void on_actionOpen_triggered();

    void on_pushButton_2_clicked();

private:
    drawable* edited_shape;
    bool changingRadius = false;
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
    ClickableLabel* myLabel;
    Point* changingPoint = nullptr;
    EditDrawableDialog* dialog;
};

#endif // MAINWINDOW_H
