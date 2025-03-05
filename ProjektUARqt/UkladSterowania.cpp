#include "ModelARX.h"
#include "RegulatorPID.h"
#include "UkladSterowania.h"

UkladSterowania::UkladSterowania(ModelARX arx_model, RegulatorPID pid, GWZ gwz)
    : model(arx_model), regulator(pid), gwz(gwz)
{
    uchyb = 0.0;
    poprzedniUchyb = 0.0;
}

UkladSterowania::UkladSterowania()
: model(), regulator(), gwz(){
    uchyb = 0.0;
    poprzedniUchyb = 0.0;
}

double UkladSterowania::symuluj(double aktualne_wejscie) {
    double wyjscie_pid;
    double uchyb = aktualne_wejscie - getPoprzedniUchyb();
    setUchyb(uchyb);
    wyjscie_pid = regulator.wykonajKrok(getUchyb());
    double wyjscie_model = model.wykonajKrok(wyjscie_pid);
    setPoprzedniUchyb(wyjscie_model);
    return wyjscie_pid;
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
