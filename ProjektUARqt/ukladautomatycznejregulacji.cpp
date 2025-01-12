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

    /*ui->customPlot->addGraph();
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
    ui->customPlot->update();*/

    ui->customPlot->addGraph(); // Wykres wyjścia modelu ARX
    ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
    ui->customPlot->xAxis->setLabel("Czas [s]");
    ui->customPlot->yAxis->setLabel("Wyjście");
    ui->customPlot->xAxis->setRange(0, 10);
    ui->customPlot->yAxis->setRange(0, 100);

    // --- TIMER ---
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &UkladAutomatycznejRegulacji::startSymulacji);

    // Dezaktywacja przycisku Stop na start
    ui->zatrzymaj->setEnabled(false);

}

UkladAutomatycznejRegulacji::~UkladAutomatycznejRegulacji()
{
    ZapisDoPliku();
    delete ui;
}

void UkladAutomatycznejRegulacji::on_zapisDoPliku_clicked()
{
    ZapisDoPliku();
}

void UkladAutomatycznejRegulacji::on_wgrajzPliku_clicked()
{
    WczytajzPliku();
}


void UkladAutomatycznejRegulacji::on_symuluj_clicked()
{
    model = ustawARX();
    pid = ustawPID();
    double wz = ui->te_wartZadana->toPlainText().toDouble();
    us = ustawUS(model, pid, wz);
    qDebug() << "wektor A: " << model->getA() << "\n";
    qDebug() << "wektor B: " << model->getB() << "\n";

    if(!timer->isActive()){
        timer->start(100);  // Timer co 100 ms

        // Dezaktywacja przycisku Start i aktywacja Stop
        ui->symuluj->setEnabled(false);
        ui->zatrzymaj->setEnabled(true);

    }

}

void UkladAutomatycznejRegulacji::ZapisDoPliku()
{
    QString nazwa = QCoreApplication::applicationDirPath() + "/konfiguracja.txt";
    QFile plik(nazwa);
    if (!plik.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Nie udało się znaleźć pliku";
        return;
    }
    QTextStream out(&plik);
    if(!(ui->te_a->toPlainText().isEmpty()))
    {
        out << "a: " << ui->te_a->toPlainText() << "\n";
    }
    else
    {
        out << "a: " << 0 << "\n";
    }
    if(!(ui->te_b->toPlainText().isEmpty()))
    {
        out << "b: " << ui->te_b->toPlainText() << "\n";
    }
    else
    {
        out << "b: " << 0 << "\n";
    }
    if(!(ui->te_opoznienie->toPlainText().isEmpty()))
    {
        out << "opoznienie: " << ui->te_opoznienie->toPlainText() << "\n";
    }
    else
    {
        out << "opoznienie: " << 0 << "\n";
    }
    if(!(ui->te_zaklocenie->toPlainText().isEmpty()))
    {
        out << "zaklocenie: " << ui->te_zaklocenie->toPlainText() << "\n";
    }
    else
    {
        out << "zaklocenie: " << 0 << "\n";
    }
    if(!(ui->te_k->toPlainText().isEmpty()))
    {
        out << "k: " << ui->te_k->toPlainText() << "\n";
    }
    else
    {
        out << "k: " << 0 << "\n";
    }
    if(!(ui->te_ti->toPlainText().isEmpty()))
    {
        out << "Ti: " << ui->te_ti->toPlainText() << "\n";
    }
    else
    {
        out << "Ti: " << 0 << "\n";
    }
    if(!(ui->te_td->toPlainText().isEmpty()))
    {
        out << "Td: " << ui->te_td->toPlainText() << "\n";
    }
    else
    {
        out << "Td: " << 0 << "\n";
    }
    if(!(ui->te_wartZadana->toPlainText().isEmpty()))
    {
        out << "wartoscZadana: " << ui->te_wartZadana->toPlainText() << "\n";
    }
    else
    {
        out << "wartoscZadana: " << 0 << "\n";
    }
    plik.close();
}

void UkladAutomatycznejRegulacji::WczytajzPliku()
{
    QString nazwa = QCoreApplication::applicationDirPath() + "/konfiguracja.txt";
    QFile plik(nazwa);
    if (!plik.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Nie udało się otworzyć pliku";
        return;
    }
    QTextStream in(&plik);
    while (!in.atEnd()) {
        QString linia = in.readLine();
        if (linia.startsWith("a:")) {
            ui->te_a->setPlainText(linia.mid(2).trimmed());
        } else if (linia.startsWith("b:")) {
            ui->te_b->setPlainText(linia.mid(2).trimmed());
        } else if (linia.startsWith("opoznienie:")) {
            ui->te_opoznienie->setPlainText(linia.section(':', 1).trimmed());
        } else if (linia.startsWith("zaklocenie:")) {
            ui->te_zaklocenie->setPlainText(linia.section(':', 1).trimmed());
        } else if (linia.startsWith("k:")) {
            ui->te_k->setPlainText(linia.section(':', 1).trimmed());
        } else if (linia.startsWith("Ti:")) {
            ui->te_ti->setPlainText(linia.section(':', 1).trimmed());
        } else if (linia.startsWith("Td:")) {
            ui->te_td->setPlainText(linia.section(':', 1).trimmed());
        } else if (linia.startsWith("wartoscZadana:")) {
            ui->te_wartZadana->setPlainText(linia.section(':', 1).trimmed());
        }
    }

    plik.close();
}


void UkladAutomatycznejRegulacji::on_wyczyscDane_clicked()
{
    ui->te_a->clear();
    ui->te_b->clear();
    ui->te_opoznienie->clear();
    ui->te_zaklocenie->clear();
    ui->te_k->clear();
    ui->te_ti->clear();
    ui->te_td->clear();
    ui->te_wartZadana->clear();
}

ModelARX *UkladAutomatycznejRegulacji::ustawARX()
{
    std::vector<double> a;
    std::vector<double> b;
    QString text_a = ui->te_a->toPlainText();
    QString text_b = ui->te_b->toPlainText();

    QStringList aList = text_a.split(" ");
    QStringList bList = text_b.split(" ");

    for (const QString &a_i : aList)
    {
        bool ok;
        double value = a_i.toDouble(&ok);
        if (ok) {
            a.push_back(value);
        }
    }

    for (const QString &b_i : bList)
    {
        bool ok;
        double value = b_i.toDouble(&ok);
        if (ok) {
            b.push_back(value);
        }
    }
    int delay = ui->te_opoznienie->toPlainText().toInt();
    double zaklocenie = ui->te_zaklocenie->toPlainText().toDouble();
    return (new ModelARX(a, b, delay, zaklocenie));
}
RegulatorPID* UkladAutomatycznejRegulacji::ustawPID()
{
    double wzmocnienie = ui->te_k->toPlainText().toDouble();
    double stala_calkowania = ui->te_k->toPlainText().toDouble();
    double stala_rozniczkowania = ui->te_k->toPlainText().toDouble();

    return (new RegulatorPID(wzmocnienie, stala_calkowania, stala_rozniczkowania));

}
UkladSterowania* UkladAutomatycznejRegulacji::ustawUS(ModelARX* model, RegulatorPID* pid, double wz)
{
    return (new UkladSterowania(*model, *pid, wz));
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

void UkladAutomatycznejRegulacji::on_zatrzymaj_clicked()
{
    if(timer->isActive()){
        timer->stop();
        ui->symuluj->setEnabled(true);
        ui->zatrzymaj->setEnabled(false);
    }
}

