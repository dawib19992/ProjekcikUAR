#ifndef OKNOPID_H
#define OKNOPID_H

#pragma once

#include <QDialog>
#include "RegulatorPID.h"

namespace Ui {
class OknoPID;
}

class OknoPID : public QDialog
{
    Q_OBJECT

public:
    explicit OknoPID(QWidget *parent = nullptr);
    ~OknoPID;

    void ustawDane(RegulatorPID pid); // wczytanie istniejących danych
    RegulatorPID pobierzDane() const;  // zwrócenie zmodyfikowanego obiektu

private:
    Ui::OknoPID *ui;
};

#endif // OKNOPID_H
