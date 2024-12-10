#pragma once
#include <deque>
#include <vector>
#include <random>
#include "ObiektWejsciaWyjscia.h"
class ModelARX : ObiektWejsciaWyjscia
{
private:
    std::vector<double> wspolczynniki_a;
    std::vector<double> wspolczynniki_b;
    std::deque<double> bufor_wejscia;
    std::deque<double> bufor_wyjscia;
    std::deque<double> bufor_opoznienia;
    int opoznienie_k;
    double odchylenie_szumu;
    std::default_random_engine generator;
    std::normal_distribution<double> rozklad_szumu;
public:
    ModelARX(std::vector<double>& a, std::vector<double>& b, int opoznienie = 0, double szum = 0.0);
    double wykonajKrok(double wejscie) override;
    void reset() override;
};

