#include "RegulatorPID.h"

RegulatorPID :: RegulatorPID(double k, double Ti, double Td, double dG, double gG)
    : wzmocnienie(k), stala_calkowania(Ti), stala_rozniczkowania(Td), suma_calkowita(0), dGranicaAW(dG), gGranicaAW(gG)
{}
RegulatorPID::RegulatorPID() {}
double RegulatorPID::wykonajKrok(double uchyb)
{
    skladnik_wzmocnienia = wzmocnienie * uchyb;

    if (stala_calkowania > 0) {
        if (!antiwindup) {
            suma_calkowita += uchyb;
        }
        skladnik_calkowania = suma_calkowita / stala_calkowania;
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
}

void RegulatorPID::setGranica(double dolna, double gorna)
{
    dGranicaAW = dolna;
    gGranicaAW = gorna;
}
