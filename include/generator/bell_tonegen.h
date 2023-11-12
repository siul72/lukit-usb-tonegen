#ifndef BELL_TONEGEN_H
#define BELL_TONEGEN_H

#include "tonegen.h"
 

class BellGenerator: public ToneGenerator
{
    private:
        int fm_Hz;
        int I0;
        double tau;
        double theta_m;
        double theta_c;
    public:
        BellGenerator(int fm_Hz, int I0, double tau);
        double generate(int fc_Hz, double timeIndexSeconds, int durationSeconds);
        
};
 
#endif
