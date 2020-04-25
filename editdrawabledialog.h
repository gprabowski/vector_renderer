#ifndef EDITDRAWABLEDIALOG_H
#define EDITDRAWABLEDIALOG_H

#include <QDialog>
#include "color.h"
namespace Ui {
class EditDrawableDialog;
}

class EditDrawableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditDrawableDialog(QWidget *parent = nullptr, Color col = Color(0,0,0), int thickness = 1);
    int getThickness();
    Color getColor();
    bool shouldDelete();
    bool isCanceled();
    ~EditDrawableDialog();

private slots:
    void on_spinBox_valueChanged(int arg1);

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::EditDrawableDialog *ui;
    int thickness;
    Color col;
    bool cancel = false;
    bool should_delete = false;
};

#endif // EDITDRAWABLEDIALOG_H
