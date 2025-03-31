#ifndef OKNOARX_H
#define OKNOARX_H

#pragma once

#include <QDialog>
#include <vector>

namespace Ui {
class OknoARX;
}

class OknoARX : public QDialog
{
    Q_OBJECT

public:
    explicit OknoARX(QWidget *parent = nullptr);
    ~OknoARX();

    void ustawDane(const std::vector<double> &a, const std::vector<double> &b, int opoznienie, double zaklocenie);
    void pobierzDane(std::vector<double> &a, std::vector<double> &b, int &opoznienie, double &zaklocenie) const;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::OknoARX *ui;
};
#endif // OKNOARX_H
