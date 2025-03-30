#include "RegulatorPID.h"

RegulatorPID :: RegulatorPID(double k, double Ti, double Td, double dG, double gG)
    : wzmocnienie(k), stala_calkowania(Ti), stala_rozniczkowania(Td), suma_calkowita(0), dGranicaAW(dG), gGranicaAW(gG)
{}
RegulatorPID::RegulatorPID() {}
double RegulatorPID::wykonajKrok(double uchyb)
{
    // Składowa proporcjonalna
    skladnik_wzmocnienia = wzmocnienie * uchyb;

    // Składowa całkująca
    if (stala_calkowania > 0) {
        if (!antiwindup) {
            if (!metoda_calkowania) {
                // Metoda standardowa (dzielenie po sumowaniu)
                suma_calkowita += uchyb;
                skladnik_calkowania = suma_calkowita / stala_calkowania;
            } else {
                // Metoda zalecana (przemnażanie w każdym kroku)
                suma_calkowita += (uchyb * (1.0 / stala_calkowania));
                skladnik_calkowania = suma_calkowita;
            }
        }
    } else {
        skladnik_calkowania = 0; // Gdy Ti = 0, całkowanie wyłączone
    }
    if (stala_rozniczkowania > 0) {
        double roznicaUchybow = uchyb - poprzedniUchyb;
        skladnik_rozniczkowania = stala_rozniczkowania * roznicaUchybow;
    }

    poprzedniUchyb = uchyb;
    wyjscie = skladnik_wzmocnienia + skladnik_calkowania + skladnik_rozniczkowania;

    if(czyAW){
        if (wyjscie < dGranicaAW) {
            wyjscie = dGranicaAW;
            antiwindup = true;  // Aktywuj anti-windup
        } else if (wyjscie > gGranicaAW) {
            wyjscie = gGranicaAW;
            antiwindup = true;  // Aktywuj anti-windup
        } else {
            antiwindup = false;  // Dezaktywuj anti-windup
        }
    }
    return wyjscie;
}

void RegulatorPID::reset()
{
	suma_calkowita = 0;
    poprzedniUchyb = 0;
}

void RegulatorPID::setGranica(double dolna, double gorna)
{
    dGranicaAW = dolna;
    gGranicaAW = gorna;
}
