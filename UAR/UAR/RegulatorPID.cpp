#include "RegulatorPID.h"
#include "ObiektWejsciaWyjscia.h"

RegulatorPID :: RegulatorPID(double k, double Ti, double Td, double czas_probkowania)
	: k(k), Ti(Ti), Td(Td), czas_probkowania(czas_probkowania), suma_calkowita(0), poprzedni_uchyb(0) {}

double RegulatorPID::wykonajKrok(double uchyb)
{
	double skladnik_wzmocnieniaP;
	double skladnik_calkowaniaI;
	double skladnik_rozniczkowaniaD;

	skladnik_wzmocnieniaP = k * uchyb;
	suma_calkowita += uchyb * czas_probkowania;

	if (Ti != 0)
	{
		skladnik_calkowaniaI = (k / Ti) * suma_calkowita;
	}
	else
	{
		skladnik_calkowaniaI = 0;
	}

	if (Td != 0)
	{
		skladnik_rozniczkowaniaD = k * Td * ((uchyb - poprzedni_uchyb) / czas_probkowania);
	}
	else
	{
		skladnik_rozniczkowaniaD = 0;
	}

	poprzedni_uchyb = uchyb;
	double wynik = skladnik_wzmocnieniaP + skladnik_calkowaniaI + skladnik_rozniczkowaniaD;

	return wynik;
}

void RegulatorPID::reset()
{
	suma_calkowita = 0;
	poprzedni_uchyb = 0;
}
