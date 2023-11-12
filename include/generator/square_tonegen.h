#ifndef SQUARE_TONEGEN_H
#define SQUARE_TONEGEN_H
 
 #include "tonegen.h"

class SquareWaveGenerator: public ToneGenerator
{
    public:
      
        double generate(int fundamentalFrequencyHz, double timeIndexSeconds, int durationSeconds);
};

#endif
