#include "ukladautomatycznejregulacji.h"
#include "ui_ukladautomatycznejregulacji.h"

UkladAutomatycznejRegulacji::UkladAutomatycznejRegulacji(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UkladAutomatycznejRegulacji)
{
    ui->setupUi(this);
    model = nullptr;
    pid = nullptr;
    us = nullptr;
}

UkladAutomatycznejRegulacji::~UkladAutomatycznejRegulacji()
{
    delete ui;
}

void UkladAutomatycznejRegulacji::on_wgrajdane_clicked()
{
    //ModelARX
    std::vector<double> a;
    std::vector<double>b;
    int delay = ui->te_opoznienie->toPlainText().toInt();
    double zaklocenie = ui->te_zaklocenie->toPlainText().toDouble();
    model = new ModelARX(a, b, delay, zaklocenie);

    //PID
    double wzmocnienie = ui->te_k->toPlainText().toDouble();
    double stala_calkowania = ui->te_k->toPlainText().toDouble();
    double stala_rozniczkowania = ui->te_k->toPlainText().toDouble();

    pid = new RegulatorPID(wzmocnienie, stala_calkowania, stala_rozniczkowania);

    //Uchyb
    double uchyb = 0.0;
    double poprzedni_uchyb = 0.0;
    double wz = ui->te_wartZadana->toPlainText().toDouble();

    Uchyb u(uchyb, poprzedni_uchyb, wz);


}

