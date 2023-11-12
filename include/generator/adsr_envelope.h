#ifndef ADSR_ENVELOP_H
#define ADSR_ENVELOP_H

#include "envelope.h"

// Attack, Decay, Sustain, Release (ADSR) Envelope: https://en.wikipedia.org/wiki/Envelope_(music)
class ADSREnvelope: public Envelope
{
    private:
        double attackDurationSeconds;
        double attackAmplitude;
        double decayDurationSeconds;
        double sustainDurationSeconds;
        double sustainAmplitude;
        double releaseDurationSeconds;
        double releaseAmplitude;
    public:
        ADSREnvelope(double durationSeconds);
        double getAmplitude(double timeIndexSeconds);
};

class BellEnvelope: public Envelope
{
    private:
        double tau; // duration it takes for the signal to decay to 1/e = 36.8%
    public:
        BellEnvelope(double tau);
        double getAmplitude(double timeIndexSeconds);
};
 
#endif
