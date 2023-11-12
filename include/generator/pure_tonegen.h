#ifndef PURE_TONEGEN_H
#define PURE_TONEGEN_H

#include "tonegen.h"

class PureToneGenerator: public ToneGenerator
{
    public:
        double generate(int toneFrequencyHz, double timeIndexSeconds, double durationSeconds);
};
 

#endif
