#include "oknoPID.h"
#include "ui_oknoPID.h"


OknoPID::OknoPID(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OknoPID)
{
    ui->setupUi(this);
    setWindowTitle("Konfiguracja Regulatora PID");
}

OknoPID::~OknoPID()
{
    delete ui;
}

void OknoPID::ustawDane(const RegulatorPID& pid)
{
    ui->spinbox_k->setValue(pid.getK());
    ui->spinbox_ti->setValue(pid.getTi());
    ui->spinbox_td->setValue(pid.getTd());
    ui->spinbox_granicad->setValue(pid.getDolna());
    ui->spinbox_granicag->setValue(pid.getGorna());
    ui->checkbox_windup->setChecked(pid.getAW());
    ui->checkbox_calka->setChecked(pid.getMetodaCalkowania());
}

RegulatorPID OknoPID::pobierzDane() const
{
    RegulatorPID pid;
    pid.setK(ui->spinbox_k->value());
    pid.setTi(ui->spinbox_ti->value());
    pid.setTd(ui->spinbox_td->value());
    pid.setGranica(ui->spinbox_granicad->value(), ui->spinbox_granicag->value());
    pid.setAW(ui->checkbox_windup->isChecked());
    pid.ustawMetodeCalkowania(ui->checkbox_calka->isChecked());
    return pid;
}

