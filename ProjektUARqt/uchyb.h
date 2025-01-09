#ifndef UCHYB_H
#define UCHYB_H

class Uchyb
{
private:
    double uchyb;
    double poprzedniUchyb;
    double wartosc_zadana;
public:
    Uchyb(double uchyb_, double poprzedniUchyb_, double wz);
    void setUchyb(double uchyb_);
    void setPoprzedniUchyb(double poprzedniUchyb_);
    double getUchyb();
    double getPoprzedniUchyb();
    void obliczUchyb();
    void reset();
};

#endif // UCHYB_H
