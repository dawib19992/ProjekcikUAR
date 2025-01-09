// TU includy:

#include <iostream>
#include <iomanip>
#include "ModelARX.h"
#include "RegulatorPID.h"
#include "UkladSterowania.h"

#define DEBUG  // ustaw na MAIN aby skompilować program docelowy / ustaw na DEBUG aby skompilować program testujacy 

#ifdef DEBUG

//Funkcje pomocnicze dla testów:

void raportBleduSekwencji(std::vector<double>& spodz, std::vector<double>& fakt)
{
	constexpr size_t PREC = 3;
	std::cerr << std::fixed << std::setprecision(PREC);
	std::cerr << "  Spodziewany:\t";
	for (auto& el : spodz)
		std::cerr << el << ", ";
	std::cerr << "\n  Faktyczny:\t";
	for (auto& el : fakt)
		std::cerr << el << ", ";
	std::cerr << std::endl << std::endl;
}

bool porownanieSekwencji(std::vector<double>& spodz, std::vector<double>& fakt)
{
	constexpr double TOL = 1e-3;	// tolerancja dla porównań zmiennoprzecinkowych
	bool result = fakt.size() == spodz.size();
	for (int i = 0; result && i < fakt.size(); i++)
		result = fabs(fakt[i] - spodz[i]) < TOL;
	return result;
}

void test_UAR_1_brakPobudzenia()
{
	//Sygnatura testu:
	std::cerr << "UAR_1 -> test zerowego pobudzenia: ";
	try
	{
		// Przygotowanie danych:
		RegulatorPID testPID(0.5, 5.0, 0.2);
		std::vector<double> a = { -0.4 };
		std::vector<double> b = { 0.6 };
		ModelARX testARX(a, b, 1, 0);
		UkladSterowania instancjaTestowa(testARX, testPID, 0);
		constexpr size_t LICZ_ITER = 30;
		std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu (tu same 0)
		std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy (tu same 0)
		std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

		// Symulacja UAR:

		for (int i = 0; i < LICZ_ITER; i++)
			faktSygWy[i] = instancjaTestowa.symuluj(sygWe[i]);

		// Walidacja poprawności i raport:
		if (porownanieSekwencji(spodzSygWy, faktSygWy))
			std::cerr << "OK!\n";
		else
		{
			std::cerr << "FAIL!\n";
			raportBleduSekwencji(spodzSygWy, faktSygWy);
		}
	}
	catch (...)
	{
		std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
	}
}

void test_UAR_1_skokJednostkowy()
{
	//Sygnatura testu:
	std::cerr << "UAR_1 -> test skoku jednostkowego: ";
	try
	{
		// Przygotowanie danych:
		RegulatorPID testPID(0.5, 5.0, 0.2);
		std::vector<double> a = { -0.4 };
		std::vector<double> b = { 0.6 };
		ModelARX testARX(a, b, 0);
		UkladSterowania instancjaTestowa(testARX, testPID, 0);
		constexpr size_t LICZ_ITER = 30;
		std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu (tu same 0)
		std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy (tu same 0)
		std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

		// Symulacja skoku jednostkowego w chwili 1. (!!i - daje 1 dla i != 0);
		for (int i = 0; i < LICZ_ITER; i++)
			sygWe[i] = !!i;
		spodzSygWy = { 0, 0.9, 0.9, 0.614, 0.6196, 0.78828, 0.864728, 0.85054, 0.849231,
			           0.881775, 0.911732, 0.924077, 0.93093, 0.941192, 0.952188, 0.960156,
			           0.965694, 0.970663, 0.975398, 0.97939, 0.982546, 0.985174, 0.987472,
			           0.989446, 0.991089, 0.992459, 0.993624, 0.994616, 0.995453, 0.996157 };

		// Symulacja UAR:

		for (int i = 0; i < LICZ_ITER; i++)
			faktSygWy[i] = instancjaTestowa.symuluj(sygWe[i]);

		// Walidacja poprawności i raport:
		if (porownanieSekwencji(spodzSygWy, faktSygWy))
			std::cerr << "OK!\n";
		else
		{
			std::cerr << "FAIL!\n";
			raportBleduSekwencji(spodzSygWy, faktSygWy);
		}
	}
	catch (...)
	{
		std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
	}
}

void test_UAR_2_skokJednostkowy()
{
	//Sygnatura testu:
	std::cerr << "UAR_2 (k = 2) -> test skoku jednostkowego: ";
	try
	{
		// Przygotowanie danych:
		RegulatorPID testPID(0.5, 5.0, 0.2);
		std::vector<double> a = { -0.4 };
		std::vector<double> b = { 0.6 };
		ModelARX testARX(a, b, 0);
		UkladSterowania instancjaTestowa(testARX, testPID, 0);
		constexpr size_t LICZ_ITER = 30;
		std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu (tu same 0)
		std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy (tu same 0)
		std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

		// Symulacja skoku jednostkowego w chwili 1. (!!i - daje 1 dla i != 0);
		for (int i = 0; i < LICZ_ITER; i++)
			sygWe[i] = !!i;
		spodzSygWy = { 0, 0.9, 0.9, 1.1, 0.814, 0.8196, 0.72584, 0.854776, 0.891326, 0.978897,
					   0.965948, 0.97393, 0.947262, 0.956628, 0.958596, 0.977871, 0.983651,
					   0.991229, 0.989057, 0.990035, 0.988989, 0.991706, 0.993317, 0.995839,
					   0.996577, 0.997323, 0.997255, 0.997632, 0.997878, 0.998425
		             };
		// Symulacja UAR:

		for (int i = 0; i < LICZ_ITER; i++)
			faktSygWy[i] = instancjaTestowa.symuluj(sygWe[i]);

		// Walidacja poprawności i raport:
		if (porownanieSekwencji(spodzSygWy, faktSygWy))
			std::cerr << "OK!\n";
		else
		{
			std::cerr << "FAIL!\n";
			raportBleduSekwencji(spodzSygWy, faktSygWy);
		}
	}
	catch (...)
	{
		std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
	}
}



int main()
{
	test_UAR_1_brakPobudzenia();
	test_UAR_1_skokJednostkowy();
	test_UAR_2_skokJednostkowy();
}

#endif


#ifdef MAIN


int main()
{
	//Twój program
}

#endif

