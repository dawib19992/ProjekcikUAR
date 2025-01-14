#include "ukladautomatycznejregulacji.h"
#include "ui_ukladautomatycznejregulacji.h"

UkladAutomatycznejRegulacji::UkladAutomatycznejRegulacji(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UkladAutomatycznejRegulacji)
{
    ui->setupUi(this);
    setFixedSize(1230, 750);
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
    model = nullptr;
    pid = nullptr;
    us = nullptr;
    gwz = nullptr;
    // Wykres wyjścia modelu ARX
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setPen(QPen(Qt::cyan));
    //Uchyb
    ui->customPlot->addGraph();
    ui->customPlot->graph(1)->setPen(QPen(Qt::red));
    //Sterowanie
    ui->customPlot->addGraph();
    ui->customPlot->graph(2)->setPen(QPen(Qt::green));
    //Regulowanie
    ui->customPlot->addGraph();
    ui->customPlot->graph(3)->setPen(QPen(Qt::blue));
    ui->customPlot->legend->setVisible(true);
    ui->customPlot->graph(0)->setName("Sterowanie");
    ui->customPlot->graph(1)->setName("Uchyb");
    ui->customPlot->graph(2)->setName("Wartość Zadana");
    ui->customPlot->graph(3)->setName("Wartość Regulowana");

    ui->customPlot->xAxis->setLabel("Czas [s]");
    ui->customPlot->yAxis->setLabel("Wyjście");
    ui->customPlot->xAxis->setRange(0, 10);
    ui->customPlot->yAxis->setRange(-5, 5);

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
        timer->start(150);  // Timer co 100 ms

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
        QMessageBox::warning(this, "Błąd otwarcia pliku", "Program nie mógł otworzyć pliku w celu zapisu konfiguracji do pliku", QMessageBox::Ok);
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
    if(!(ui->amplituda->toPlainText().isEmpty()))
    {
        out << "amplituda: " << ui->amplituda->toPlainText() << "\n";
    }
    else
    {
        out << "amplituda: " << 0 << "\n";
    }
    if(!(ui->wypelnienie->toPlainText().isEmpty()))
    {
        out << "wypelnienie: " << ui->wypelnienie->toPlainText() << "\n";
    }
    else
    {
        out << "wypelnienie: " << 0 << "\n";
    }
    if(!(ui->czas_aktywacji->toPlainText().isEmpty()))
    {
        out << "czas_aktywacji: " << ui->czas_aktywacji->toPlainText() << "\n";
    }
    else
    {
        out << "czas_aktywacji: " << 0 << "\n";
    }
    if(!(ui->okres->toPlainText().isEmpty()))
    {
        out << "okres: " << ui->okres->toPlainText() << "\n";
    }
    else
    {
        out << "okres: " << 0 << "\n";
    }
    if(!(ui->comboGWZ->currentText().isEmpty()))
    {
        out << "typ: " << ui->comboGWZ->currentText()<< "\n";
    }
    else
    {
        out << "okres: " << "skok" << "\n";
    }
    plik.close();
    QMessageBox::information(this, "Zapis konfiguracji", "Konfiguracja została pomyślnie zapisana do pliku.", QMessageBox::Ok);
}

void UkladAutomatycznejRegulacji::WczytajzPliku()
{
    QString nazwa = QCoreApplication::applicationDirPath() + "/konfiguracja.txt";
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
            ui->te_opoznienie->setPlainText(linia.section(':', 1).trimmed());
        } else if (linia.startsWith("k:")) {
            ui->te_k->setPlainText(linia.section(':', 1).trimmed());
        } else if (linia.startsWith("Ti:")) {
            ui->te_ti->setPlainText(linia.section(':', 1).trimmed());
        } else if (linia.startsWith("Td:")) {
            ui->te_td->setPlainText(linia.section(':', 1).trimmed());
        } else if (linia.startsWith("amplituda:")) {
            ui->amplituda->setPlainText(linia.section(':', 1).trimmed());
        } else if (linia.startsWith("wypelnienie:")) {
            ui->wypelnienie->setPlainText(linia.section(':', 1).trimmed());
        } else if (linia.startsWith("czas_aktywacji:")) {
            ui->czas_aktywacji->setPlainText(linia.section(':', 1).trimmed());
        } else if (linia.startsWith("okres:")) {
            ui->okres->setPlainText(linia.section(':', 1).trimmed());
        } else if(linia.startsWith("typ: ")){
            QString typ = linia.section(':', 1).trimmed();
            int index = ui->comboGWZ->findText(typ);
            if(index != -1)
                ui->comboGWZ->setCurrentIndex(index);
        }
    }
    plik.close();
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
    double wyjscie_pid = us->getPoprzedniUchyb();
    uchyb = wartZadana - wyjscie_pid;
    us->symuluj(wartZadana);

    // ARX
    ui->customPlot->graph(0)->addData(time, wyjscie_arx);
    // Uchyb
    ui->customPlot->graph(1)->addData(time, uchyb);
    // Sterowanie
    ui->customPlot->graph(2)->addData(time, wartZadana);
    // Regulator
    ui->customPlot->graph(3)->addData(time, wyjscie_pid);

    if (time > ui->customPlot->xAxis->range().upper)
    {
        ui->customPlot->xAxis->setRange(time, 10, Qt::AlignRight);
    }
    ui->customPlot->replot();
    qDebug() << "zaklocenie: " <<model->getZaklocenie();
    qDebug() << "zaklocenie us: " <<us->model.getZaklocenie();
    qDebug() << "isZaklocenie: " <<isZaklocenie;
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

void UkladAutomatycznejRegulacji::on_zaklocenie_clicked()
{
    if(isZaklocenie)
    {
        isZaklocenie = false;
    }
    else
        isZaklocenie = true;
}

