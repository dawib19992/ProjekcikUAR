#pragma once
#include "ModelARX.h"
#include "ObiektWejsciaWyjscia.h"
#include "GWZ.h"
#include "RegulatorPID.h"
class UkladSterowania
{
private:
    ModelARX& model;
    RegulatorPID& regulator;
    GWZ& generator_wz;
    double wartosc_zadana;
public:
    UkladSterowania(ModelARX& arx_model, RegulatorPID& pid, GWZ& generator);
    void wykonajKrok(double& aktualne_wyjscie, double& sygnal_sterowania);
};

