#ifndef VIOLIN_TONEGEN_H
#define VIOLIN_TONEGEN_H

#include "tonegen.h"

class ViolinGenerator: public ToneGenerator
{
    public:
       
        double generate(int fundamentalFrequencyHz, double timeIndexSeconds, int durationSeconds);
};

#endif
