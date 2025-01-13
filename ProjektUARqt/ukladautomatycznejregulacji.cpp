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
    gwz = nullptr;
    // Wykres wyjścia modelu ARX
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
    //Uchyb
    ui->customPlot->addGraph();
    ui->customPlot->graph(1)->setPen(QPen(Qt::red));
    //Sterowanie
    ui->customPlot->addGraph();
    ui->customPlot->graph(2)->setPen(QPen(Qt::green));
    //Regulowanie
    ui->customPlot->addGraph();
    ui->customPlot->graph(3)->setPen(QPen(Qt::cyan));
    ui->customPlot->legend->setVisible(true);
    ui->customPlot->graph(0)->setName("Wartość Regulowana");
    ui->customPlot->graph(1)->setName("Uchyb");
    ui->customPlot->graph(2)->setName("Wartość Zadana");
    ui->customPlot->graph(3)->setName("Sterowanie");

    ui->customPlot->xAxis->setLabel("Czas [s]");
    ui->customPlot->yAxis->setLabel("Wyjście");
    ui->customPlot->xAxis->setRange(-5, 10);
    ui->customPlot->yAxis->setRange(-2, 5);

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
    double stala_calkowania = ui->te_ti->toPlainText().toDouble();
    double stala_rozniczkowania = ui->te_td->toPlainText().toDouble();

    return (new RegulatorPID(wzmocnienie, stala_calkowania, stala_rozniczkowania));

}
UkladSterowania* UkladAutomatycznejRegulacji::ustawUS(ModelARX* model, RegulatorPID* pid, GWZ* gwz)
{
    return (new UkladSterowania(*model, *pid, *gwz));
}
GWZ* UkladAutomatycznejRegulacji::ustawGWZ()
{
    TypSygnalu typ;
    QString typSygnalu = ui->comboGWZ->currentText();
    if(typSygnalu == "skok"){
        typ = TypSygnalu::skok;
    }
    if(typSygnalu == "sinusoida"){
        typ = TypSygnalu::sinusoida;
    }
    if(typSygnalu == "prostokatny"){
        typ = TypSygnalu::prostokatny;
    }
    double amplituda = ui->amplituda->toPlainText().toDouble();
    int czas = ui->czas_aktywacji->toPlainText().toInt();
    double okres = ui->okres->toPlainText().toDouble();
    double wypelnienie = ui->wypelnienie->toPlainText().toDouble();
    return (new GWZ(typ, amplituda, czas, okres, wypelnienie));
}


void UkladAutomatycznejRegulacji::startSymulacji()
{
    time+=0.1;
    double uchyb = 0.0;
    double wartZadana = gwz->pobierzWartoscZadana(time);
    double wyjscie_arx = us->symuluj(wartZadana);
    double wyjscie_pid = us->getPoprzedniUchyb();
    uchyb = wartZadana - wyjscie_pid;
    //ARX
    ui->customPlot->graph(0)->addData(time,wyjscie_arx);
    //Uchyb
    ui->customPlot->graph(1)->addData(time, uchyb);
    //Sterowanie
    ui->customPlot->graph(2)->addData(time, wartZadana);
    //Regulator
    ui->customPlot->graph(3)->addData(time, wyjscie_pid);

    ui->customPlot->xAxis->setRange(0,time);
    ui->customPlot->replot();
    //ui->customPlot->rescaleAxes();

    if(time > ui->customPlot->xAxis->range().upper)
    {
        ui->customPlot->xAxis->setRange(time, 10, Qt::AlignRight);
    }

    qDebug() << "wektor A: " << model->getA();
    qDebug() << "wektor B: " << model->getB();
    qDebug() << "opoznienie: " << model->getOpoznienie();
    qDebug() << "zaklocenie: " << model->getZaklocenie();
    qDebug() << "k: " << pid->getK();
    qDebug() << "Ti: " << pid->getTi();
    qDebug() << "Td: " << pid->getTd();
    qDebug() << "Typ: " << gwz->getTyp();
}

void UkladAutomatycznejRegulacji::on_zatrzymaj_clicked()
{
    if(timer->isActive()){
        timer->stop();
        ui->symuluj->setEnabled(true);
        ui->zatrzymaj->setEnabled(false);
    }
}

void UkladAutomatycznejRegulacji::on_wgrajDane_clicked()
{
    model = ustawARX();
    pid = ustawPID();
    gwz = ustawGWZ();
    us = ustawUS(model, pid, gwz);
}

