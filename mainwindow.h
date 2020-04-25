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
    enum mode {line, circle, polygon};
    ~MainWindow();

private slots:
    void label_clicked();
    void label_options();
    void on_drawpolygon_clicked();
    void on_drawline_clicked();
    void on_drawcircle_clicked();
    void on_actionReset_triggered();
    void on_pushButton_clicked();

private:
    Color col = Color(0, 0, 0);
    mode _mode = line;
    bool from_start = true;
    drawable* current;
    std::vector<std::pair<Point* , drawable*>> points;
    string type;
    vector<drawable*> objects;
    Ui::MainWindow *ui;
    QImage _current;
    ClickableLabel* myLabel;
    Point* changingPoint = nullptr;
    EditDrawableDialog* dialog;
};

#endif // MAINWINDOW_H
