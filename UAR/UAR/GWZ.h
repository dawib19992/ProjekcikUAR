#pragma once
class GWZ {
private:
	double amplituda;
	int czas_aktywacji;
	int aktualny_czas;

public:
    GWZ(double amplituda_, int czas_aktywacji_);

    double pobierzWartoscZadana();

    void zwiekszCzas();

    void reset();
};