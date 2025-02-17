#pragma once
#include <QString>

enum TypSygnalu { skok, sinusoida, prostokatny };
class GWZ {
private:
	double amplituda;
	int czas_aktywacji; //skok
	int aktualny_czas;
    TypSygnalu typ;
    double okres; //sinus
    double wypelnienie; //prostokatny

public:
    GWZ(TypSygnalu typ_, double amplituda_, int czas_aktywacji_ = 0, double okres_ = 0.0, double wypelnienie_ = 0.0);
    double pobierzWartoscZadana(double czas);
    void reset();
};
