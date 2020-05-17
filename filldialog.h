#ifndef FILLDIALOG_H
#define FILLDIALOG_H

#include <QDialog>
#include "color.h"

namespace Ui {
class FillDialog;
}

class FillDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FillDialog(QWidget *parent = nullptr, Color col = Color(0,0,0));
    Color getColor();
    bool isSolid();
    bool isCanceled();
    ~FillDialog();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_radioButton_clicked();

    void on_pushButton_4_clicked();

    void on_radioButton_2_clicked();

private:
    Color col;
    bool solid = true;
    bool cancel = false;
    Ui::FillDialog *ui;
};

#endif // FILLDIALOG_H
