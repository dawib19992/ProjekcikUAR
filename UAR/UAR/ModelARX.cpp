#include "ModelARX.h"
ModelARX::ModelARX(const std::vector<double>& a, const std::vector<double>& b, int opoznienie = 0, double szum = 0.0)
    : wspolczynniki_a(a), wspolczynniki_b(b), opoznienie_k(opoznienie), odchylenie_szumu(szum),
    rozklad_szumu(0.0, szum) {
    bufor_wejscia.resize(b.size(), 0.0);
    bufor_wyjscia.resize(a.size(), 0.0);
    bufor_opoznienia.resize(opoznienie_k, 0.0);
}

double ModelARX::wykonajKrok(double wejscie) {

    //Aktualizacja buforu opóŸnienia

    bufor_opoznienia.push_back(wejscie);
    double opoznione_wejscie = bufor_opoznienia.front();
    bufor_opoznienia.pop_front();

    //Aktualizacja buforu wejœciowego

    bufor_wejscia.push_back(opoznione_wejscie);
    bufor_wejscia.pop_front();

    double wyjscie = 0.0;

    // Wk³ad ze wspó³czynników A i sygna³ów wejœciowych

    for (size_t i = 0; i < wspolczynniki_a.size(); ++i) {
        wyjscie -= wspolczynniki_a[i] * bufor_wyjscia[bufor_wyjscia.size() - 1 - i];
    }

    // Wk³ad ze wspó³czynników B i sygna³ów wejœciowych

    for (size_t i = 0; i < wspolczynniki_b.size(); ++i) {
        wyjscie += wspolczynniki_b[i] * bufor_wejscia[bufor_wejscia.size() - 1 - i];
    }

    //Dodanie szumu

    if (odchylenie_szumu > 0.0) {
        wyjscie += rozklad_szumu(generator);
    }

    // Aktualizacja buforu wyjœciowego

    bufor_wyjscia.push_back(wyjscie);
    bufor_wyjscia.pop_front();

    return wyjscie;
}

void ModelARX::reset() {
    std::fill(bufor_wejscia.begin(), bufor_wejscia.end(), 0.0);
    std::fill(bufor_wyjscia.begin(), bufor_wyjscia.end(), 0.0);
    std::fill(bufor_opoznienia.begin(), bufor_opoznienia.end(), 0.0);
}
