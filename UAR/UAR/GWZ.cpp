#include <iostream>
#include "GWZ.h"

using namespace std;

//Zwrot wartoœci zadaniej w chwilii i

double GWZ::pobierzWartoscZadana()
{
	return (aktualny_czas >= czas_aktywacji) ? amplituda : 0.0;
}

void GWZ::zwiekszCzas() {
	++aktualny_czas;
}

void GWZ::reset() {
	aktualny_czas = 0;
}

GWZ::GWZ(double amplituda_, int czas_aktywacji_)
	: amplituda(amplituda_), czas_aktywacji(czas_aktywacji_), aktualny_czas(0)
{}