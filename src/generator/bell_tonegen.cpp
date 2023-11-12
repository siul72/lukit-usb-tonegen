 #include "generator/bell_tonegen.h"

BellGenerator::BellGenerator(int fm_Hz, int I0, double tau): fm_Hz(fm_Hz), I0(I0), tau(tau), theta_m(-M_PI/2), theta_c(-M_PI/2)
{
}

double BellGenerator::generate(int fc_Hz, double timeIndexSeconds, double durationSeconds)
{
    double At = exp(-timeIndexSeconds / this->tau);
    double It = this->I0 * exp(-timeIndexSeconds / this->tau);
    double result = At * cos(2 * M_PI * fc_Hz * timeIndexSeconds + It * cos(2 * M_PI * this->fm_Hz * timeIndexSeconds + this->theta_m) + this->theta_c);

    return result;
}
 