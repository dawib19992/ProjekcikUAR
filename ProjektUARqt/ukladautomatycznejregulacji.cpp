#include "ukladautomatycznejregulacji.h"
#include "ui_ukladautomatycznejregulacji.h"
#include "ModelARX.h"

UkladAutomatycznejRegulacji::UkladAutomatycznejRegulacji(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UkladAutomatycznejRegulacji)
{
    ui->setupUi(this);
    model = nullptr;
    pid = nullptr;
    us = nullptr;

    // --- Konfiguracja wykresu ---
    ui->customPlot->addGraph(); // Wykres wyjścia modelu ARX
    ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
    ui->customPlot->xAxis->setLabel("Czas [s]");
    ui->customPlot->yAxis->setLabel("Wyjście");
    ui->customPlot->xAxis->setRange(0, 10);
    ui->customPlot->yAxis->setRange(0, 100);

    // --- TIMER ---
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &UkladAutomatycznejRegulacji::startSymulacji);

    // --- Podłączenie przycisków ---
    connect(ui->start_symulacji, &QPushButton::clicked, this, &UkladAutomatycznejRegulacji::start_symulacji);
    connect(ui->stop_symulacji, &QPushButton::clicked, this, &UkladAutomatycznejRegulacji::stop_symulacji);

    // Dezaktywacja przycisku Stop na start
    ui->stop_symulacji->setEnabled(false);

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

void UkladAutomatycznejRegulacji::startSymulacji()
{
    static std::vector<double> a = {0.8};
    static std::vector<double> b = {0.2};
    static ModelARX modelARX(a,b,1,0.1);

    static RegulatorPID regulatorPID(1.0,0.5,0.1);
    static double poprzednieWyjscie =0.0;

    double zadanaWartosc =1.0;
    double uchyb=zadanaWartosc-poprzednieWyjscie;

    double sterowanie = regulatorPID.wykonajKrok(uchyb);
    double wyjscie = modelARX.wykonajKrok(sterowanie);

    ui->customPlot->graph(0)->addData(time,wyjscie);
    ui->customPlot->xAxis->setRange(0,time);
    ui->customPlot->replot();
    ui->customPlot->rescaleAxes();
    time+=0.1;
}

void UkladAutomatycznejRegulacji::start_symulacji ()
{
    if(!timer->isActive()){
        timer->start(100);  // Timer co 100 ms

        // Dezaktywacja przycisku Start i aktywacja Stop
        ui->start_symulacji->setEnabled(false);
        ui->stop_symulacji->setEnabled(true);

    }
}
void UkladAutomatycznejRegulacji::stop_symulacji()
{
    if(timer->isActive()){
        timer->stop();
        ui->start_symulacji->setEnabled(true);
        ui->stop_symulacji->setEnabled(false);
    }
}




