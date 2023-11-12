#ifndef CHIRP_TONEGEN_H
#define CHIRP_TONEGEN_H

#include "tonegen.h" 

class ChirpGenerator: public ToneGenerator
{
    public:
        double generate(int toneFrequencyHz, double timeIndexSeconds, double durationSeconds);
};
 
#endif
