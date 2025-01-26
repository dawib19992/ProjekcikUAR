#pragma once

class RegulatorPID
{
	double wzmocnienie;
	double stala_calkowania;
	double stala_rozniczkowania;
	double suma_calkowita;
    double skladnik_wzmocnienia =0.0;
    double skladnik_calkowania =0.0;
    double skladnik_rozniczkowania =0.0;
    double poprzedniUchyb = 0.0;
    double wyjscie;
    double dGranicaAW;
    double gGranicaAW;
    bool antiwindup;

public:

    RegulatorPID(double k, double Ti = 0.0, double Td =0.0, double dG = -1000.0, double gG = 1000.0);
    double wykonajKrok(double uchyb);
    double getK() {return skladnik_wzmocnienia;}
    double getTi() {return skladnik_calkowania;}
    double getTd() {return skladnik_rozniczkowania;}
	void reset();
    void setGranica(double dolna, double gorna);
};

