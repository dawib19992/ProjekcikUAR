#include "ukladautomatycznejregulacji.h"
#include "ui_ukladautomatycznejregulacji.h"

const QString nazwa = "konfiguracja.txt";

UkladAutomatycznejRegulacji::UkladAutomatycznejRegulacji(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UkladAutomatycznejRegulacji)
{
    ui->setupUi(this);
    setFixedSize(1600, 900);
    ustawShortcuty();
    ustawWykresy();
    ui->zaklocenie_wartosc->setVisible(false);
    model = nullptr;
    pid = nullptr;
    us = nullptr;
    gwz = nullptr;
    ui->gorna->setMaximum(1000);
    ui->dolna->setMinimum(-1000);
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &UkladAutomatycznejRegulacji::startSymulacji);
    ui->zatrzymaj->setEnabled(false);
    ui->ukryjLegendy->setEnabled(false);

}

UkladAutomatycznejRegulacji::~UkladAutomatycznejRegulacji()
{
    delete ui;
}

void UkladAutomatycznejRegulacji::startSymulacji()
{
    time += 0.1;
    double zaklocenie;
    if(isZaklocenie == false)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<double> dist(0.0, 0.05);
        zaklocenie = dist(gen);
        us->model.setZaklocenie(zaklocenie);
    }
    else
    {
        zaklocenie = 0.0;
        us->model.setZaklocenie(zaklocenie);
    }
    double wartZadana = us->gwz.pobierzWartoscZadana(time);
    double wyjscie_arx = us->model.wykonajKrok(wartZadana);
    double wyjscie_pid = us->regulator.wykonajKrok(uchyb);
    double wzmocnienie = us->regulator.getK();
    double Ti = us->regulator.getTi();
    double Td = us->regulator.getTd();
    uchyb = wartZadana - wyjscie_arx;
    us->symuluj(wartZadana);

    //ARX
    ui->customPlot->graph(0)->addData(time, wyjscie_arx);
    //Uchyb
    ui->customPlot_uchyb->graph(0)->addData(time, uchyb);
    //Wartość Zadana
    ui->customPlot->graph(1)->addData(time, wartZadana);
    //PID
    ui->customPlot_pid->graph(0)->addData(time, wyjscie_pid);

    ui->customPlot_pid->graph(1)->addData(time, wzmocnienie);

    ui->customPlot_pid->graph(2)->addData(time, Ti);

    ui->customPlot_pid->graph(3)->addData(time, Td);

    if (time > ui->customPlot->xAxis->range().upper)
    {
        ui->customPlot->xAxis->setRange(time, 10, Qt::AlignRight);
        ui->customPlot_uchyb->xAxis->setRange(time, 10, Qt::AlignRight);
        ui->customPlot_pid->xAxis->setRange(time, 10, Qt::AlignRight);
    }
    ui->customPlot->replot();
    ui->customPlot_pid->replot();
    ui->customPlot_uchyb->replot();
    ui->customPlot->xAxis->rescale();
    ui->customPlot_pid->xAxis->rescale();
    ui->customPlot_uchyb->xAxis->rescale();
    ui->customPlot->yAxis->rescale();
    ui->customPlot_pid->yAxis->rescale();
    ui->customPlot_uchyb->yAxis->rescale();
    ui->zaklocenie_wartosc->setText("zakłócenie: " + QString::number(us->model.getZaklocenie()));
}

void UkladAutomatycznejRegulacji::on_symuluj_clicked()
{
    if(isWgrane == true)
    {
        if(!timer->isActive()){
            timer->start(100);  // Timer co 100 ms

            // Dezaktywacja przycisku Start i aktywacja Stop
            ui->symuluj->setEnabled(false);
            ui->zatrzymaj->setEnabled(true);

        }
          ui->ukryjLegendy->setEnabled(true);
    }
    else
    {
        QMessageBox::warning(this, "Błąd startu symulacji", "Przed rozpoczęciem symulacji upewnij się że dane zostały poprawnie wgrane do programu");
    }


}

void UkladAutomatycznejRegulacji::on_zatrzymaj_clicked()
{
    if(timer->isActive()){
        timer->stop();
        ui->symuluj->setEnabled(true);
        ui->zatrzymaj->setEnabled(false);
    }
}


void UkladAutomatycznejRegulacji::on_wyczyscDane_clicked()
{
    ui->te_a->clear();
    ui->te_b->clear();
    ui->te_opoznienie->clear();
    ui->te_k->clear();
    ui->te_ti->clear();
    ui->te_td->clear();
    ui->amplituda->clear();
    ui->czas_aktywacji->clear();
    ui->wypelnienie->clear();
    ui->okres->clear();
    QString skok = "skok";
    int index = ui->comboGWZ->findText(skok);
    ui->comboGWZ->setCurrentIndex(index);
    ui->gorna->clear();
    ui->dolna->clear();
}

ModelARX *UkladAutomatycznejRegulacji::ustawARX()
{
    bool ok;
    std::vector<double> a;
    std::vector<double> b;
    QString text_a = ui->te_a->toPlainText();
    QString text_b = ui->te_b->toPlainText();

    QStringList aList = text_a.split(" ");
    QStringList bList = text_b.split(" ");

    for (const QString &a_i : aList)
    {
        double value = a_i.toDouble(&ok);
        if (ok) {
            a.push_back(value);
        }
        else
        {
            QMessageBox::warning(this, "Błąd wartości", "Podaj poprawną wartość wektora A!", QMessageBox::Ok);
        }
    }

    for (const QString &b_i : bList)
    {
        double value = b_i.toDouble(&ok);
        if (ok) {
            b.push_back(value);
        }
        else
        {
             QMessageBox::warning(this, "Błąd wartości", "Podaj poprawną wartość wektora B!", QMessageBox::Ok);
        }
    }
    int delay = ui->te_opoznienie->value();
    if(isZaklocenie)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<double> dist(0.0, 0.05);
        double zaklocenie = dist(gen);
        return (new ModelARX(a, b, delay, zaklocenie));
    }
    return (new ModelARX(a, b, delay,0));
}
RegulatorPID* UkladAutomatycznejRegulacji::ustawPID()
{
    double wzmocnienie = ui->te_k->value();
    double stala_calkowania = ui->te_ti->value();
    double stala_rozniczkowania = ui->te_td->value();
    double gorna = ui->gorna->value();
    double dolna = ui->dolna->value();
    return (new RegulatorPID(wzmocnienie, stala_calkowania, stala_rozniczkowania, dolna, gorna));

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
    double amplituda = ui->amplituda->value();
    int czas = ui->czas_aktywacji->value();
    double okres = ui->okres->value();
    double wypelnienie = ui->wypelnienie->value();
    return (new GWZ(typ, amplituda, czas, okres, wypelnienie));
}

UkladSterowania* UkladAutomatycznejRegulacji::ustawUS(ModelARX* model, RegulatorPID* pid, GWZ* gwz)
{
    return (new UkladSterowania(*model, *pid, *gwz));
}

void UkladAutomatycznejRegulacji::on_zaklocenie_clicked()
{
    if(isZaklocenie)
    {
        isZaklocenie = false;
        ui->zaklocenie_wartosc->setVisible(true);
    }
    else
    {
        isZaklocenie = true;
        ui->zaklocenie_wartosc->setVisible(false);
    }
}

void UkladAutomatycznejRegulacji::on_wgrajDane_clicked()
{
    model = ustawARX();
    pid = ustawPID();
    gwz = ustawGWZ();
    us = ustawUS(model, pid, gwz);
    double dolnaGranica = ui->dolna->value();
    double gornaGranica = ui->gorna->value();
    pid->setGranica(dolnaGranica, gornaGranica);
    isWgrane = true;
}

void UkladAutomatycznejRegulacji::on_zapisDoPliku_clicked()
{
    ZapisDoPliku();
}

void UkladAutomatycznejRegulacji::ZapisDoPliku()
{
    bool ok;
    QFile plik(nazwa);
    if (!plik.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Błąd otwarcia pliku", "Program nie mógł otworzyć pliku w celu zapisu konfiguracji do pliku", QMessageBox::Ok);
    }
    QTextStream out(&plik);
    if(!(ui->te_a->toPlainText().isEmpty()))
    {
        out << "a: " << ui->te_a->toPlainText().toDouble(&ok) << "\n";
    }
    else
    {
        out << "a: " << 0 << "\n";
    }
    if(!(ui->te_b->toPlainText().isEmpty()))
    {
        out << "b: " << ui->te_b->toPlainText().toDouble(&ok) << "\n";
    }
    else
    {
        out << "b: " << 0 << "\n";
    }
    out << "opoznienie: " << ui->te_opoznienie->value() << "\n";
    out << "k: " << ui->te_k->value() << "\n";
    out << "Ti: " << ui->te_ti->value() << "\n";
    out << "Td: " << ui->te_td->value() << "\n";
    out << "amplituda: " << ui->amplituda->value() << "\n";
    out << "wypelnienie: " << ui->wypelnienie->value() << "\n";
    out << "czas_aktywacji: " << ui->czas_aktywacji->value() << "\n";
    out << "okres: " << ui->okres->value() << "\n";
    out << "typ: " << ui->comboGWZ->currentText()<< "\n";
    out << "dolna: " << ui->dolna->value()<< "\n";
    out << "gorna: " << ui->gorna->value() << "\n";
    QMessageBox::information(this, "Zapis do pliku", "Konfiguracja została zapisana do pliku", QMessageBox::Ok);
    plik.close();

}

void UkladAutomatycznejRegulacji::on_wgrajzPliku_clicked()
{
    WczytajzPliku();
}

void UkladAutomatycznejRegulacji::WczytajzPliku()
{
    QFile plik(nazwa);
    if (!plik.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Błąd otwarcia pliku", "Program nie mógł otworzyć pliku w celu wczytania konfiguracji", QMessageBox::Ok);
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
            ui->te_opoznienie->setValue(linia.section(':', 1).trimmed().toDouble());
        } else if (linia.startsWith("k:")) {
            ui->te_k->setValue(linia.section(':', 1).trimmed().toDouble());
        } else if (linia.startsWith("Ti:")) {
            ui->te_ti->setValue(linia.section(':', 1).trimmed().toDouble());
        } else if (linia.startsWith("Td:")) {
            ui->te_td->setValue(linia.section(':', 1).trimmed().toDouble());
        } else if (linia.startsWith("amplituda:")) {
            ui->amplituda->setValue(linia.section(':', 1).trimmed().toDouble());
        } else if (linia.startsWith("wypelnienie:")) {
            ui->wypelnienie->setValue(linia.section(':', 1).trimmed().toDouble());
        } else if (linia.startsWith("czas_aktywacji:")) {
            ui->czas_aktywacji->setValue(linia.section(':', 1).trimmed().toDouble());
        } else if (linia.startsWith("okres:")) {
            ui->okres->setValue(linia.section(':', 1).trimmed().toDouble());
        } else if(linia.startsWith("typ: ")){
            QString typ = linia.section(':', 1).trimmed();
            int index = ui->comboGWZ->findText(typ);
            if(index != -1)
                ui->comboGWZ->setCurrentIndex(index);
        } else if(linia.startsWith("dolna: ")){
            ui->dolna->setValue(linia.section(':', 1).trimmed().toDouble());
        } else if(linia.startsWith("gorna: ")){
            ui->gorna->setValue(linia.section(':', 1).trimmed().toDouble());
        }
    }
    plik.close();
}

void UkladAutomatycznejRegulacji::ustawShortcuty()
{
    QShortcut* zapis_skrot = new QShortcut(QKeySequence("Ctrl+S"), this);
    QShortcut* wczytaj_skrot = new QShortcut(QKeySequence("Ctrl+L"), this);
    QShortcut* start_skrot = new QShortcut(QKeySequence("Ctrl+F2"),this);
    QShortcut* stop_skrot = new QShortcut(QKeySequence("Ctrl+F3"), this);
    QShortcut* wyczysc_skrot = new QShortcut(QKeySequence("Ctrl+F4"), this);
    QShortcut* wgraj_skrot = new QShortcut(QKeySequence("Ctrl+F1"), this);
    connect(zapis_skrot, &QShortcut::activated, this, &UkladAutomatycznejRegulacji::ZapisDoPliku);
    connect(wczytaj_skrot, &QShortcut::activated, this, &UkladAutomatycznejRegulacji::WczytajzPliku);
    connect(start_skrot, &QShortcut::activated, this, &UkladAutomatycznejRegulacji::on_symuluj_clicked);
    connect(stop_skrot, &QShortcut::activated, this, &UkladAutomatycznejRegulacji::on_zatrzymaj_clicked);
    connect(wyczysc_skrot, &QShortcut::activated, this, &UkladAutomatycznejRegulacji::on_wyczyscDane_clicked);
    connect(wgraj_skrot, &QShortcut::activated, this, &UkladAutomatycznejRegulacji::on_wgrajDane_clicked);
}


void UkladAutomatycznejRegulacji::ustawWykresy()
{
    // Wykres wyjścia modelu ARX
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setPen(QPen(Qt::red, 1.5));
    //Wartość Zadana
    ui->customPlot->addGraph();
    ui->customPlot->graph(1)->setPen(QPen(Qt::blue,1.5));
    //Uchyb
    ui->customPlot_uchyb->addGraph();
    ui->customPlot_uchyb->graph(0)->setPen(QPen(Qt::green, 1.5));
    //Sterowanie
    ui->customPlot_pid->addGraph();
    ui->customPlot_pid->graph(0)->setPen(QPen(Qt::blue));
    //Wzmocnienie
    ui->customPlot_pid->addGraph();
    ui->customPlot_pid->graph(1)->setPen(QPen(Qt::magenta));
    //Ti - stala calkowania
    ui->customPlot_pid->addGraph();
    ui->customPlot_pid->graph(2)->setPen(QPen(Qt::red));
    //Td - stala rozniczkowania
    ui->customPlot_pid->addGraph();
    ui->customPlot_pid->graph(3)->setPen(QPen(Qt::green));
    //Legenda Głównego wykresu
    ui->customPlot->legend->setVisible(true);
    ui->customPlot->graph(0)->setName("Wartość Regulowana");
    ui->customPlot->graph(1)->setName("Wartość Zadana");
    //Legenda Uchybu
    ui->customPlot_uchyb->legend->setVisible(true);
    ui->customPlot_uchyb->graph(0)->setName("Uchyb");
    //Legenda PID
    ui->customPlot_pid->legend->setVisible(true);
    ui->customPlot_pid->graph(0)->setName("Sterowanie");
    ui->customPlot_pid->graph(1)->setName("Wzmocnienie");
    ui->customPlot_pid->graph(2)->setName("Stała Całkowania");
    ui->customPlot_pid->graph(3)->setName("Stała Różniczkownia");

    ui->customPlot->xAxis->setLabel("Czas [s]");
    ui->customPlot->yAxis->setLabel("Wyjście");
    ui->customPlot->xAxis->setRange(0, 10);
    ui->customPlot->yAxis->setRange(-5, 5);

}


void UkladAutomatycznejRegulacji::on_ukryjLegendy_clicked()
{
    if(isLegenda == true)
    {
        ui->customPlot->legend->setVisible(false);
        ui->customPlot_pid->legend->setVisible(false);
        ui->customPlot_uchyb->legend->setVisible(false);
        isLegenda = false;
        ui->ukryjLegendy->setText("Pokaż legendy");
    }
    else
    {
        ui->customPlot->legend->setVisible(true);
        ui->customPlot_pid->legend->setVisible(true);
        ui->customPlot_uchyb->legend->setVisible(true);
        isLegenda = true;
        ui->ukryjLegendy->setText("Ukryj legendy");
    }
}

