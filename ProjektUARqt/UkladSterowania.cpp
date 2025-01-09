#include "ModelARX.h"
#include "RegulatorPID.h"
#include "UkladSterowania.h"

UkladSterowania::UkladSterowania(ModelARX& arx_model, RegulatorPID& pid, Uchyb &u)
    : model(arx_model), regulator(pid), uchyb(u) {}

double UkladSterowania::symuluj(double aktualne_wejscie) {
    uchyb.setUchyb(aktualne_wejscie - uchyb.getPoprzedniUchyb());

    uchyb.setPoprzedniUchyb(model.wykonajKrok(regulator.wykonajKrok(uchyb.getUchyb())));

    return regulator.wykonajKrok(uchyb.getUchyb());;
}


//int main()
//{
//    std::vector<double> a = { 0.6, -0.2 };
//    std::vector<double> b = { 0.2, 0.1 };
//    int opoznienie = 1;
//    double zaklocenie = 1;
//    ModelARX model(a, b, opoznienie, zaklocenie);
//
//    // Stabilne parametry regulatora PID
//    double k = 1;
//    double Ti = 1;
//    double Td = 0.7;
//    RegulatorPID regulator(k, Ti, Td);
//
//    double amplituda = 1;
//    int czas_aktywacji = 5;
//    double okres = 5;
//    double wypelnienie = 0.5;
//
//    GWZ generator(TypSygnalu::skok, amplituda, czas_aktywacji, okres, wypelnienie);
//
//    // Uk�ad sterowania
//    UkladSterowania uklad(model, regulator, generator);
//
//    double aktualne_wyjscie = 0.0;
//    double sygnal_sterowania = 0.0;
//
//    for (int i = 0; i < 50; ++i) {
//        uklad.symuluj(aktualne_wyjscie);
//
//        std::cout << "Krok: " << i + 1
//            << ", Wartosc zadana: " << generator.pobierzWartoscZadana()
//            << ", Wyjscie: " << aktualne_wyjscie << std::endl;
//    }
//
//    return 0;
//}
        
