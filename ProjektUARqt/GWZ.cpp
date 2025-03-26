#include "GWZ.h"
#include <QtMath>

GWZ::GWZ(TypSygnalu typ_, double amplituda_, int czas_aktywacji_, double okres_, double wypelnienie_, double skladowa_stala_)
    : amplituda(amplituda_), czas_aktywacji(czas_aktywacji_), typ(typ_), okres(okres_), wypelnienie(wypelnienie_), skladowa_stala(skladowa_stala_)
{}


GWZ::GWZ() {}

double GWZ::pobierzWartoscZadana(double czas)
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
    {
        if(czas >= czas_aktywacji)
        {
            wartosc = amplituda * std::sin((2*M_PI/okres) * czas);
            if(std::abs(wartosc) < 0.00000000001)
            {
                wartosc = 0.0;
            }
        }
        break;
    }
    case TypSygnalu::prostokatny:
        if (czas >= czas_aktywacji)
        {
            // Oblicz resztę z dzielenia czasu przez okres
            double faza = std::fmod(czas, okres);

            // Uwzględnij wypełnienie sygnału
            if (faza < (okres * wypelnienie))
            {
                wartosc = amplituda + skladowa_stala;  // A + S
            }
            else
            {
                wartosc = skladowa_stala;  // S
            }
        }
        break;

    default:
        wartosc = 0.0;
        break;
    };
    return wartosc;
}


void GWZ::reset() {
	aktualny_czas = 0;
}

