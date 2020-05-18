#ifndef BORDERFILLDIALOG_H
#define BORDERFILLDIALOG_H

#include <QDialog>
#include "color.h"

namespace Ui {
class BorderFillDialog;
}

class BorderFillDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BorderFillDialog(QWidget *parent = nullptr, Color col = Color(255, 255, 255));
    bool isCanceled() {return cancel;}
    Color getBorder() { return border; }
    Color getFill() { return fill; }
    ~BorderFillDialog();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::BorderFillDialog *ui;
    bool cancel = false;
    Color border;
    Color fill;
};

#endif // BORDERFILLDIALOG_H
