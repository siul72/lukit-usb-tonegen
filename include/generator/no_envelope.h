#ifndef NO_ENVELOPE_H
#define NO_ENVELOPE_H
 
 #include "envelope.h" 

class NoEnvelope: public Envelope
{
    public:
        double getAmplitude(double timeIndexSeconds);
};
 
#endif
