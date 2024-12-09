#include "UkladSterowania.h"
#include <iostream>

UkladSterowania::UkladSterowania(ModelARX& arx_model, RegulatorPID& pid, GWZ& generator)
    : model(arx_model), regulator(pid), generator_wz(generator), wartosc_zadana(0.0) {}

void UkladSterowania::wykonajKrok(double& aktualne_wyjscie, double& sygnal_sterowania) {
    wartosc_zadana = generator_wz.pobierzWartoscZadana();
    double uchyb = wartosc_zadana - aktualne_wyjscie;
    sygnal_sterowania = regulator.wykonajKrok(uchyb);
    aktualne_wyjscie = model.wykonajKrok(sygnal_sterowania);
    generator_wz.zwiekszCzas();
}
