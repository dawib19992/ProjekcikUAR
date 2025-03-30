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
    bool czyAW;
    bool metoda_calkowania;

public:

    RegulatorPID(double k, double Ti = 0.0, double Td =0.0, double dG = -1000.0, double gG = 1000.0);
    RegulatorPID();
    double wykonajKrok(double uchyb);
    double getK() {return skladnik_wzmocnienia;}
    double getTi() {return skladnik_calkowania;}
    double getTd() {return skladnik_rozniczkowania;}
    void setAW(bool aw) {czyAW = aw;}
	void reset();
    void setGranica(double dolna, double gorna);
    void setK(double k) {wzmocnienie = k;}
    void setTi(double Ti) {stala_calkowania = Ti;}
    void setTd(double Td) {stala_rozniczkowania =Td;}
    void ustawMetodeCalkowania(bool metoda) { metoda_calkowania=metoda;}
};

