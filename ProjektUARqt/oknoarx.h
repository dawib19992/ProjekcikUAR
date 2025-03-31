#ifndef OKNOARX_H
#define OKNOARX_H

#pragma once

#include <QDialog>
#include <vector>

namespace Ui
{
class OknoARX;
}

class OknoARX : public QDialog
{
    Q_OBJECT

public:
    explicit OknoARX(QWidget *parent = nullptr);
    ~OknoARX();

    QString getA();
    QString getB();

    void setA(QString wartosc);
    void setB(QString wartosc);

    int getOpoznienie();
    double getZaklocenie();

    void setOpoznienie(int wartosc_op);
    void setZaklocenie(double wartosc_zak);






private slots:


private:
    Ui::OknoARX *ui;
};
#endif // OKNOARX_H
