#pragma once
#include <deque>
#include <vector>
#include <random>
#include "ObiektWejsciaWyjscia.h"
class ModelARX
{
private:
    std::vector<double> a;
    std::vector<double> b;
    std::deque<double> bufor_sterowania;
    std::deque<double> bufor_opoznienia;
    int opoznienie_k;
    double zaklocenie;
public:
    ModelARX(std::vector<double>& a, std::vector<double>& b, int opoznienie, double zaklocenie_);
    double wykonajKrok(double wejscie);
};

