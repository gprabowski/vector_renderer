#include "borderfilldialog.h"
#include "ui_borderfilldialog.h"
#include "QColorDialog"


BorderFillDialog::BorderFillDialog(QWidget *parent, Color col) :
    QDialog(parent),
    ui(new Ui::BorderFillDialog),
    border(col),
    fill(col)

{
    ui->setupUi(this);
    QPixmap color(ui->label->width(), ui->label->height());
    color.fill(QColor(col.r, col.g, col.b));
    ui->label->setPixmap(color);
    ui->label_2->setPixmap(color);
}

BorderFillDialog::~BorderFillDialog()
{
    delete ui;
}

void BorderFillDialog::on_pushButton_clicked()
{
    //choose color of border

    QColor color = QColorDialog::getColor();
    if(color.isValid() )
    {
        QPixmap color_view(ui->label->width(), ui->label->height());
        color_view.fill(color);
        ui->label->setPixmap(color_view);
        int _r, _g, _b;
        color.getRgb(&_r, &_g, &_b);
        border = Color(_r, _g, _b);
    }
}

void BorderFillDialog::on_pushButton_2_clicked()
{
    //choose color of inside

    QColor color = QColorDialog::getColor();
    if(color.isValid() )
    {
        QPixmap color_view(ui->label->width(), ui->label->height());
        color_view.fill(color);
        ui->label_2->setPixmap(color_view);
        int _r, _g, _b;
        color.getRgb(&_r, &_g, &_b);
        fill = Color(_r, _g, _b);
    }
}

void BorderFillDialog::on_pushButton_4_clicked()
{
    //cancel
    cancel = true;
    this->close();
}

void BorderFillDialog::on_pushButton_3_clicked()
{
    //ok
    cancel = false;
    this->close();
}
