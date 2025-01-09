#pragma once
#include "ModelARX.h"
#include "RegulatorPID.h"
#include "uchyb.h"
class UkladSterowania
{
private:
    ModelARX& model;
    RegulatorPID& regulator;
    Uchyb& uchyb;
public:
    UkladSterowania(ModelARX& arx_model, RegulatorPID& pid, Uchyb& u);
    double symuluj(double aktualne_wejscie);

};

