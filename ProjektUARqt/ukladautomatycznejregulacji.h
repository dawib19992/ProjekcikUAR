#ifndef UKLADAUTOMATYCZNEJREGULACJI_H
#define UKLADAUTOMATYCZNEJREGULACJI_H

#include <QMainWindow>
#include "ModelARX.h"
#include "RegulatorPID.h"
#include "UkladSterowania.h"
#include <QTimer>
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
    void on_wgrajdane_clicked();

private:
    Ui::UkladAutomatycznejRegulacji *ui;
    ModelARX* model;
    RegulatorPID* pid;
    UkladSterowania* us;

    QTimer *timer;
    double time;
    void startSymulacji();
};
#endif // UKLADAUTOMATYCZNEJREGULACJI_H
