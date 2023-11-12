#ifndef VIOLIN_TONEGEN_H
#define VIOLIN_TONEGEN_H

#include "tonegen.h"

class ViolinGenerator: public ToneGenerator
{
    public:
        double generate(int toneFrequencyHz, double timeIndexSeconds, double durationSeconds);
};
 

#endif
