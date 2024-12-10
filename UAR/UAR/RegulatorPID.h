#pragma once
#include "ObiektWejsciaWyjscia.h"

class RegulatorPID : public ObiektWejsciaWyjscia
{
	double k;
	double Ti;
	double Td;
	double czas_probkowania;
	double suma_calkowita;
	double poprzedni_uchyb;

public:

	RegulatorPID(double k, double Ti = 0.0, double Td = 0.0, double czas_probkowania = 1.0);
	//RegulatorPID(double k, double Ti, double Td, double czas_probkowania);
	double wykonajKrok(double uchyb) override;
	void reset() override;
};

