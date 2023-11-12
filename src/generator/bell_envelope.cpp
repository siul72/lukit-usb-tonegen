 #include "generator/bell_envelope.h"

BellEnvelope::BellEnvelope(double tau): tau(tau)
{
}

double BellEnvelope::getAmplitude(double timeIndexSeconds)
{
    double result = exp(-timeIndexSeconds / this->tau);

    return result;
}
 