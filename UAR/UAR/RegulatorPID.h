#pragma once
#include "ObiektWejsciaWyjscia.h"

class RegulatorPID : public ObiektWejsciaWyjscia
{
	double wzmocnienie;
	double stala_calkowania;
	double stala_rozniczkowania;
	double suma_calkowita;
	double poprzedni_uchyb;

public:

	RegulatorPID(double k, double Ti, double Td);
	double wykonajKrok(double uchyb) override;
	void reset() override;
};

