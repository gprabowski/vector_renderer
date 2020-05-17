#include "editdrawabledialog.h"
#include "ui_editdrawabledialog.h"
#include <qcolordialog.h>
EditDrawableDialog::EditDrawableDialog(QWidget *parent, Color col, int thickness) :
    QDialog(parent),
    ui(new Ui::EditDrawableDialog),
    thickness(thickness),
    col(col)
{
    ui->setupUi(this);
    ui->spinBox->setValue(thickness);
    QPixmap color(ui->label->width(), ui->label->height());
    color.fill(QColor(col.r, col.g, col.b));
    ui->label->setPixmap(color);
}

EditDrawableDialog::~EditDrawableDialog()
{
    delete ui;
}

void EditDrawableDialog::on_spinBox_valueChanged(int arg1)
{
    thickness = arg1;
}

void EditDrawableDialog::on_pushButton_3_clicked()
{
    QColor color = QColorDialog::getColor();
    if(color.isValid() )
    {
        QPixmap color_view(ui->label->width(), ui->label->height());
        color_view.fill(color);
        ui->label->setPixmap(color_view);
        int _r, _g, _b;
        color.getRgb(&_r, &_g, &_b);
        col = Color(_r, _g, _b);
    }
}

void EditDrawableDialog::on_pushButton_4_clicked()
{
    //delete
    should_delete = true;
    this->close();
}

int EditDrawableDialog::getThickness()
{
    return thickness;
}

Color EditDrawableDialog::getColor()
{
    return col;
}

bool EditDrawableDialog::shouldDelete()
{
    return should_delete;
}


void EditDrawableDialog::on_pushButton_2_clicked()
{
    cancel = false;
    this->close();
}

bool EditDrawableDialog::isCanceled()
{
    return cancel;
}

void EditDrawableDialog::on_pushButton_clicked()
{
    cancel = true;
    this->close();
}
