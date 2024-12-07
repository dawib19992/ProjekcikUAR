#pragma once
class ObiektWejsciaWyjscia {
public:
    virtual double wykonajKrok(double sygnal) = 0;
    virtual void reset() = 0;
    virtual ~ObiektWejsciaWyjscia() = default;
};