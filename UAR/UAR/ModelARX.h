#pragma once
#include <vector>
#include <deque>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <iomanip>
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
    ModelARX(std::vector<double>& a_, std::vector<double>& b_, int opoznienie = 1, double zaklocenie_ = 0.0);
    double wykonajKrok(double wejscie);
};