#pragma once
#include "ModelARX.h"
#include "RegulatorPID.h"
class UkladSterowania
{
private:
    ModelARX model;
    RegulatorPID regulator;
    double uchyb;
    double poprzedniUchyb;
    double wartosc_zadana;
public:
    UkladSterowania(ModelARX arx_model, RegulatorPID pid, double wz = 1.0);
    double symuluj(double aktualne_wejscie);
    void setUchyb(double uchyb_);
    void setPoprzedniUchyb(double poprzedniUchyb_);
    double getUchyb();
    double getPoprzedniUchyb();
    void reset();
};

