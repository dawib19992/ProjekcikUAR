#include "RegulatorPID.h"

RegulatorPID :: RegulatorPID(double k, double Ti, double Td)
	: wzmocnienie(k), stala_calkowania(Ti), stala_rozniczkowania(Td), suma_calkowita(0)
{}
double RegulatorPID::wykonajKrok(double uchyb)
{
    double skladnik_wzmocnieniaP = wzmocnienie * uchyb;

    double skladnik_calkowaniaI = 0.0;
    if (stala_calkowania > 0) {
        suma_calkowita += uchyb;
        skladnik_calkowaniaI = suma_calkowita / stala_calkowania;
    }

    double skladnik_rozniczkowaniaD = 0.0;
    if (stala_rozniczkowania > 0) {
        double roznicaUchybow = uchyb - poprzedniUchyb;
        skladnik_rozniczkowaniaD = stala_rozniczkowania * roznicaUchybow;
    }

    poprzedniUchyb = uchyb;
    double wynik = skladnik_wzmocnieniaP + skladnik_calkowaniaI + skladnik_rozniczkowaniaD;

    return wynik;
}

void RegulatorPID::reset()
{
	suma_calkowita = 0;
}
