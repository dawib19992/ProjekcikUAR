#include <iostream>
#include <vector>
#include "GWZ.h"
#include "ModelARX.h"
#include "RegulatorPID.h"
#include "UkladSterowania.h"

UkladSterowania::UkladSterowania(ModelARX& arx_model, RegulatorPID& pid, GWZ& generator)
    : model(arx_model), regulator(pid), generator_wz(generator), wartosc_zadana(0.0) {}

void UkladSterowania::wykonajKrok(double& aktualne_wyjscie, double& sygnal_sterowania) {
    wartosc_zadana = generator_wz.pobierzWartoscZadana();
    double uchyb = wartosc_zadana - aktualne_wyjscie;
    sygnal_sterowania = regulator.wykonajKrok(uchyb);
    aktualne_wyjscie = model.wykonajKrok(sygnal_sterowania);
    generator_wz.zwiekszCzas();
}

int main()
{
    std::vector<double> a = { 0.3, -0.2 };
    std::vector<double> b = { 0.2, 0.1 };
    int opoznienie = 1;
    double zaklocenie = 1;
    ModelARX model(a, b, opoznienie, zaklocenie);

    // Stabilne parametry regulatora PID
    double k = 1;
    double Ti = 1;
    double Td = 0.5;
    RegulatorPID regulator(k, Ti, Td);

    double amplituda = 1;
    int czas_aktywacji = 5;
    double okres = 5;
    double wypelnienie = 0.5;

    GWZ generator(TypSygnalu::skok, amplituda, czas_aktywacji, okres, wypelnienie);

    // Uk³ad sterowania
    UkladSterowania uklad(model, regulator, generator);

    double aktualne_wyjscie = 0.0;
    double sygnal_sterowania = 0.0;

    for (int i = 0; i < 50; ++i) {
        uklad.wykonajKrok(aktualne_wyjscie, sygnal_sterowania);

        std::cout << "Krok: " << i + 1
            << ", Wartosc zadana: " << generator.pobierzWartoscZadana()
            << ", Wyjscie: " << aktualne_wyjscie << std::endl;
    }

    return 0;
}
        