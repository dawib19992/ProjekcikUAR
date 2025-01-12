#pragma once

class RegulatorPID
{
	double wzmocnienie;
	double stala_calkowania;
	double stala_rozniczkowania;
	double suma_calkowita;
	double poprzedniUchyb = 0.0;

public:

	RegulatorPID(double k, double Ti = 0.0, double Td =0.0);
	double wykonajKrok(double uchyb);
	void reset();
};

