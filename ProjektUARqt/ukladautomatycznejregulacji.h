#ifndef UKLADAUTOMATYCZNEJREGULACJI_H
#define UKLADAUTOMATYCZNEJREGULACJI_H

#include <QMainWindow>
#include "ModelARX.h"
#include "RegulatorPID.h"
#include "UkladSterowania.h"
#include "GWZ.h"
#include <QShortcut>
QT_BEGIN_NAMESPACE
namespace Ui {
class UkladAutomatycznejRegulacji;
}
QT_END_NAMESPACE

class UkladAutomatycznejRegulacji : public QMainWindow
{
    Q_OBJECT

public:
    UkladAutomatycznejRegulacji(QWidget *parent = nullptr);
    ~UkladAutomatycznejRegulacji();

private slots:

    void on_zapisDoPliku_clicked();

    void on_wgrajzPliku_clicked();

    void on_symuluj_clicked();

    void on_wyczyscDane_clicked();

    void ZapisDoPliku();

    void WczytajzPliku();
    void startSymulacji();
    ModelARX* ustawARX();
    RegulatorPID* ustawPID();
    UkladSterowania* ustawUS(ModelARX* model, RegulatorPID* pid, GWZ* gwz);
    GWZ* ustawGWZ();

    void on_zatrzymaj_clicked();

    void on_wgrajDane_clicked();

    void on_zaklocenie_clicked();

private:
    Ui::UkladAutomatycznejRegulacji *ui;
    ModelARX* model;
    RegulatorPID* pid;
    UkladSterowania* us;
    GWZ* gwz;
    double uchyb = 0.0;
    bool isZaklocenie = true;
    QTimer *timer;
    double time;
};
#endif // UKLADAUTOMATYCZNEJREGULACJI_H
