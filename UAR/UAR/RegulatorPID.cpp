#include "RegulatorPID.h"
#include "UkladSterowania.h"

RegulatorPID :: RegulatorPID(double k, double Ti, double Td)
	: wzmocnienie(k), stala_calkowania(Ti), stala_rozniczkowania(Td), suma_calkowita(0), poprzedni_uchyb(0) {}

double RegulatorPID::wykonajKrok(double uchyb)
{

    // Sk³adowe regulatora PID
    double skladnik_wzmocnieniaP = wzmocnienie * uchyb;

    double skladnik_calkowaniaI = 0.0;
    if (stala_calkowania > 0) {
        suma_calkowita += uchyb;
        skladnik_calkowaniaI = suma_calkowita / stala_calkowania;
    }

    double skladnik_rozniczkowaniaD = 0.0;
    if (stala_rozniczkowania > 0) {
        skladnik_rozniczkowaniaD = stala_rozniczkowania * (uchyb - poprzedni_uchyb);
    }

    poprzedni_uchyb = uchyb;

    // Wynik dzia³ania regulatora PID
    double wynik = skladnik_wzmocnieniaP + skladnik_calkowaniaI + skladnik_rozniczkowaniaD;

    return wynik;
}

void RegulatorPID::reset()
{
	suma_calkowita = 0;
	poprzedni_uchyb = 0;
}
