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
    double getK() {return wzmocnienie;}
    double getTi() {return stala_calkowania;}
    double getTd() {return stala_rozniczkowania;}
	void reset();
};

