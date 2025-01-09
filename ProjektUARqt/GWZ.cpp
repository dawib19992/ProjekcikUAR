#include "GWZ.h"
#include <cmath>
using namespace std;

//Zwrot warto�ci zadaniej w chwilii i

GWZ::GWZ(TypSygnalu typ_, double amplituda_, int czas_aktywacji_, double okres_, double wypelnienie_)
	:typ(typ_), amplituda(amplituda_), czas_aktywacji(czas_aktywacji_), okres(okres_), wypelnienie(wypelnienie_), aktualny_czas(0)
{}

double GWZ::pobierzWartoscZadana()
{
	double wartosc = 0.0;
	switch (typ)
	{
	case TypSygnalu::skok:
		if (aktualny_czas >= czas_aktywacji)
		{
			wartosc = amplituda;
		}
		else
		{
			wartosc = 0.0;
		}
		break;
	case TypSygnalu::sinusoida:
		wartosc = amplituda * sin((2 * 3.14 * (aktualny_czas % (int)okres)) / okres);
		break;
	case TypSygnalu::prostokatny:
		if (aktualny_czas % int(okres) < (wypelnienie * okres))
		{
			wartosc = amplituda;
		}
		else
		{
			wartosc = 0;
		}
		break;
	default:
		wartosc = 0.0;
		break;
	};

	return wartosc;


}

void GWZ::zwiekszCzas() {
	++aktualny_czas;
}

void GWZ::reset() {
	aktualny_czas = 0;
}

