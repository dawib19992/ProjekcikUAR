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

    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);
    ui->customPlot->graph()->setLineStyle(QCPGraph::lsLine);
    ui->customPlot->xAxis->setLabel("t [s]");
    ui->customPlot->yAxis->setLabel("Wyjście");
    ui->customPlot->xAxis->setRange(0,100);
    ui->customPlot->yAxis->setRange(0,5);

    QVector<double> x= {1,2,3,4,5,6,7,8,9,10},y={0.856,0.907,0.945,1.053,1.200,1.200,1.200,1.200,1.200,1.200};
    ui->customPlot->graph(0)->setData(x,y);
    ui->customPlot->rescaleAxes();
    ui->customPlot->replot();
    ui->customPlot->update();

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

    //Sprzężenie Zwrotne
    double wz = ui->te_wartZadana->toPlainText().toDouble();

    us = new UkladSterowania(*model, *pid, wz);


}

