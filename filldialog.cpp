#include "filldialog.h"
#include "ui_filldialog.h"
#include <QColorDialog>
#include <QFileDialog>


FillDialog::FillDialog(QWidget *parent, Color col) :
    QDialog(parent),
    col(col),
    ui(new Ui::FillDialog)
{
    ui->setupUi(this);
    QPixmap color(ui->label->width(), ui->label->height());
    color.fill(QColor(col.r, col.g, col.b));
    ui->label->setPixmap(color);
}

Color FillDialog::getColor() { return col; }
bool FillDialog::isCanceled() { return cancel; }

FillDialog::~FillDialog()
{
    delete ui;
}

void FillDialog::on_pushButton_2_clicked()
{
    cancel = false;
    this->close();
}

void FillDialog::on_pushButton_3_clicked()
{
    cancel = true;
    this->close();
}

void FillDialog::on_pushButton_clicked()
{
    fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "/home/", tr("Image Files (*.jpg)"));
    if(fileName.isEmpty())
        return;
    ui->label_2->setText(fileName);
}

void FillDialog::on_radioButton_clicked()
{
    solid = false;
    ui->radioButton_2->setChecked(false);
}

void FillDialog::on_radioButton_2_clicked()
{
    solid = true;
    ui->radioButton->setChecked(false);
}

void FillDialog::on_pushButton_4_clicked()
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

bool FillDialog::isSolid() { return solid; }
QString FillDialog::getFilename() { return fileName; }

