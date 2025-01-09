#include "uchyb.h"

Uchyb::Uchyb(double uchyb_, double poprzedniUchyb_, double wz)
{
    uchyb = uchyb_;
    poprzedniUchyb = poprzedniUchyb_;
    wartosc_zadana = wz;
}

void Uchyb::setUchyb(double uchyb_)
{
    uchyb = uchyb_;
}
void Uchyb::setPoprzedniUchyb(double poprzedniUchyb_)
{
    poprzedniUchyb = poprzedniUchyb_;
}
double Uchyb::getUchyb()
{
    return uchyb;
}
double Uchyb::getPoprzedniUchyb()
{
    return poprzedniUchyb;
}
void Uchyb::obliczUchyb()
{
    poprzedniUchyb = uchyb;
    uchyb = wartosc_zadana - poprzedniUchyb;
}
void Uchyb::reset()
{
    uchyb = 0.0;
    poprzedniUchyb = 0.0;
}
