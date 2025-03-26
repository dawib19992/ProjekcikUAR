#include "ModelARX.h"  
#include <vector>

ModelARX::ModelARX(std::vector<double>& a_, std::vector<double>& b_, int opoznienie, double zaklocenie_ /*,double interwal*/)
    : a(a_), b(b_), opoznienie_k(opoznienie), zaklocenie(zaklocenie_) /*,interwal(interwal)*/ {
    bufor_sterowania = std::deque<double>(b.size() + opoznienie_k, 0.0);
    bufor_opoznienia = std::deque<double>(a.size(), 0.0);
}

ModelARX::ModelARX() {}

double ModelARX::wykonajKrok(double wejscie) {

    bufor_sterowania.push_front(wejscie);

    if (bufor_sterowania.size() > a.size() + opoznienie_k)
    {
        bufor_sterowania.resize(bufor_sterowania.size() - 1);
    }

    double wyjscie = zaklocenie;
    for (size_t i = 0; i < b.size(); i++) {
        wyjscie += b[i] * bufor_sterowania[opoznienie_k + i];
    }

    for (size_t i = 0; i < a.size(); i++) {
        wyjscie -= a[i] * bufor_opoznienia[i];
    }

    bufor_opoznienia.push_front(wyjscie);
    if (bufor_opoznienia.size() > a.size()) {
        bufor_opoznienia.resize(bufor_opoznienia.size() - 1);
    }
    return wyjscie;
}

void ModelARX::setZaklocenie(double zak)
{
    zaklocenie = zak;
}

/*void ModelARX::setInterwal(double inter)
{
    interwal = inter;
}*/
