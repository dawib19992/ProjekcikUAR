#include "oknoarx.h"
#include "ui_oknoarx.h"
#include <QStringList>

OknoARX::OknoARX(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OknoARX)
{
    ui->setupUi(this);
    connect(ui->button_zatwiedz,&QPushButton::clicked,this,&OknoARX::accept);
    connect(ui->button_anuluj,&QPushButton::clicked,this,&OknoARX::reject);

    ui->spinbox_zaklocenie->setMinimum(0.0);
    ui->spinbox_zaklocenie->setMaximum(1000.0);
    ui->spinbox_opoznienie->setMinimum(1.0);
    ui->spinbox_opoznienie->setMaximum(1000.0);

}



OknoARX::~OknoARX()
{
    delete ui;
}

QString OknoARX::getA()
{
    return ui->text_a->text();
}
QString OknoARX::getB()
{
    return ui->text_b->text();
}

void OknoARX::setA(QString wartosc_a)
{
    ui->text_a->setText(wartosc_a);
}
void OknoARX::setB(QString wartosc_b)
{
    ui->text_b->setText(wartosc_b);
}

int OknoARX::getOpoznienie()
{
    return ui->spinbox_opoznienie->value();
}
double OknoARX::getZaklocenie()
{
    return ui->spinbox_zaklocenie->value();
}

void OknoARX::setOpoznienie(int opoz)
{
    ui->spinbox_opoznienie->setValue(opoz);
}
void OknoARX::setZaklocenie(double zak)
{
    ui->spinbox_zaklocenie->setValue(zak);
}

