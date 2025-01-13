#include "GWZ.h"
#define _USE_MATH_DEFINES
#include <math.h>

GWZ::GWZ(TypSygnalu typ_, double amplituda_, int czas_aktywacji_, double okres_, double wypelnienie_)
    :amplituda(amplituda_), czas_aktywacji(czas_aktywacji_), aktualny_czas(0), typ(typ_), okres(okres_), wypelnienie(wypelnienie_)
{}

double GWZ::pobierzWartoscZadana(int czas)
{
	double wartosc = 0.0;
	switch (typ)
	{
	case TypSygnalu::skok:
        if (czas >= czas_aktywacji)
		{
			wartosc = amplituda;
		}
		else
		{
			wartosc = 0.0;
		}
		break;
	case TypSygnalu::sinusoida:
        wartosc = amplituda * sin((2 * 3.14 * (czas % (int)okres)) / okres);
		break;
	case TypSygnalu::prostokatny:
        if (czas % int(okres) < (wypelnienie * okres))
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

