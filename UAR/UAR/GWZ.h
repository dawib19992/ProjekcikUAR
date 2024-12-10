#pragma once
class GWZ {
private:
	double amplituda;
	int czas_aktywacji; //skok
	int aktualny_czas;

    enum TypSygnalu {skok, sinusoida, prostokatny};
    TypSygnalu typ;

    double okres; //sinus
    double wypelnienie; //prostokatny

public:
    GWZ(TypSygnalu typ_, double amplituda_, int czas_aktywacji_ = 0, double okres_ = 0.0, double wypelnienie_ = 0.0);

    double pobierzWartoscZadana();

    void zwiekszCzas();

    void reset();
};