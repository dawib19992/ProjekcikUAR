#include <iostream>
#include <vector>
#include "GWZ.h"
#include "ModelARX.h"
#include "RegulatorPID.h"
#include "UkladSterowania.h"

//!Klasa sprzê¿enia zwrotnego!
//!Prawdopodobnie, a raczej napewno tu gdzieœ mamy b³¹d!

UkladSterowania::UkladSterowania(ModelARX& arx_model, RegulatorPID& pid, double wz)
    : model(arx_model), regulator(pid) 
{
    wartosc_zadana = wz;
    uchyb = 0.0;
    poprzedniUchyb = 0.0;
}


double UkladSterowania::symuluj(double aktualne_wejscie) {
    setUchyb(aktualne_wejscie - getPoprzedniUchyb());

    double sygnalwyj = regulator.wykonajKrok(getUchyb());

    setPoprzedniUchyb(model.wykonajKrok(sygnalwyj));
    return sygnalwyj;
}

void UkladSterowania::setUchyb(double uchyb_)
{
    uchyb = uchyb_;
}

void UkladSterowania::setPoprzedniUchyb(double poprzedniUchyb_)
{
    poprzedniUchyb = poprzedniUchyb_;
}

double UkladSterowania::getUchyb()
{
    return uchyb;
}

double UkladSterowania::getPoprzedniUchyb()
{
    return poprzedniUchyb;
}

void UkladSterowania::reset()
{
    poprzedniUchyb = 0.0;
    uchyb = 0.0;
}      