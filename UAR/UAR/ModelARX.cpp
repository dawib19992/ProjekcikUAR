#include "ModelARX.h"  
#include <vector>
#include <iomanip>

ModelARX::ModelARX(std::vector<double>& a_, std::vector<double>& b_, int opoznienie, double zaklocenie_)
    : a(a_), b(b_), opoznienie_k(opoznienie), zaklocenie(zaklocenie_) {
    bufor_sterowania = std::deque<double>(a.size() + opoznienie_k, 0.0);
    bufor_opoznienia = std::deque<double>(a.size(), 0.0);
}

double ModelARX::wykonajKrok(double wejscie) {

    bufor_sterowania.push_front(wejscie);

    // Utrzymanie odpowiedniej wielko�ci bufora sterowania
    if (bufor_sterowania.size() > b.size() + opoznienie_k) {
        bufor_sterowania.pop_back();
    }

    // Oblicz wyj�cie uwzgl�dniaj�c zak��cenia, wsp�czynniki b i a
    double wyjscie = zaklocenie;

    // Sk�adnik wej�ciowy (b * u[k])
    for (size_t i = 0; i < b.size(); i++) {
        if (i + opoznienie_k < bufor_sterowania.size()) {
            wyjscie += b[i] * bufor_sterowania[i + opoznienie_k];
        }
    }

    // Sk�adnik op�niony (a * y[k])
    for (size_t i = 0; i < a.size(); i++) {
        if (i < bufor_opoznienia.size()) {
            wyjscie += a[i] * bufor_opoznienia[i];
        }
    }

    // Aktualizacja bufora op�nienia
    bufor_opoznienia.push_front(wyjscie);
    if (bufor_opoznienia.size() > a.size()) {
        bufor_opoznienia.pop_back();
    }

    return wyjscie;
}
