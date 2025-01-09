#pragma once
class RegulatorPID
{
	double wzmocnienie;
	double stala_calkowania;
	double stala_rozniczkowania;
	double suma_calkowita;
    double poprzedni_uchyb = 0.0;

public:

	RegulatorPID(double k, double Ti, double Td);
    double wykonajKrok(double uchyb) ;
    void reset() ;
};

