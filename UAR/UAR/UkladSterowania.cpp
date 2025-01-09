#include <iostream>
#include <vector>
#include "GWZ.h"
#include "ModelARX.h"
#include "RegulatorPID.h"
#include "UkladSterowania.h"

UkladSterowania::UkladSterowania(ModelARX& arx_model, RegulatorPID& pid, double wz)
    : model(arx_model), regulator(pid) 
{
    wartosc_zadana = wz;
    uchyb = 0.0;
    poprzedniUchyb = 0.0;
}

double UkladSterowania::symuluj(double aktualne_wejscie) {
    // Obliczenie uchybu
    setUchyb(wartosc_zadana - aktualne_wejscie);

    // Obliczenie sterowania z regulatora PID
    double sterowanie = regulator.wykonajKrok(getUchyb());

    // Obliczenie odpowiedzi modelu ARX
    double wyjscie = model.wykonajKrok(sterowanie);

    // Aktualizacja poprzedniego uchybu
    setPoprzedniUchyb(wyjscie);

    return wyjscie;
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

void UkladSterowania::obliczUchyb()
{
    uchyb = wartosc_zadana - poprzedniUchyb;
    poprzedniUchyb = uchyb;
    
}

void UkladSterowania::reset()
{
    poprzedniUchyb = 0.0;
    uchyb = 0.0;
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
//    double k = 2;
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
//    // Uk³ad sterowania
//    UkladSterowania uklad(model, regulator, 1.0);
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
        