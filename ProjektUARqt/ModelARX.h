#pragma once
#include <deque>
#include <vector>
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
    ModelARX(std::vector<double>& a_, std::vector<double>& b_, int opoznienie = 0, double zaklocenie_ = 0.0);
    double wykonajKrok(double wejscie);
};
