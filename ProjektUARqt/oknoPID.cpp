#include "oknoPID.h"
#include "ui_oknoPID.h"

OknoPID::OknoPID(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OknoPID)
{
    ui->setupUi(this);
}

OknoPID::~OknoPID()
{
    delete ui;
}

void OknoPID::ustawDane(RegulatorPID pid)
{
    ui->spinK->setValue(pid.getK());
    ui->spinTi->setValue(pid.getTi());
    ui->spinTd->setValue(pid.getTd());
    ui->spinDolna->setValue(pid.getDolna());
    ui->spinGorna->setValue(pid.getGorna());
    ui->checkAW->setChecked(pid.getAW());
    ui->checkCalkowanie->setChecked(pid.getMetodaCalkowania());
}

RegulatorPID OknoPID::pobierzDane() const
{
    RegulatorPID pid;
    pid.setK(ui->spinK->value());
    pid.setTi(ui->spinTi->value());
    pid.setTd(ui->spinTd->value());
    pid.setGranica(ui->spinDolna->value(), ui->spinGorna->value());
    pid.setAW(ui->checkAW->isChecked());
    pid.ustawMetodeCalkowania(ui->checkCalkowanie->isChecked());
    return pid;
}
