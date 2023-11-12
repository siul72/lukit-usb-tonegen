#ifndef BELL_ENVELOP_H
#define BELL_ENVELOP_H

#include "envelope.h"

class BellEnvelope: public Envelope
{
    private:
        double tau; // duration it takes for the signal to decay to 1/e = 36.8%
    public:
        BellEnvelope(double tau);
        double getAmplitude(double timeIndexSeconds);
};
 
#endif
