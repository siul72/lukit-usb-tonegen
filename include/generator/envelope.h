#ifndef ENVELOPE_H
#define ENVELOPE_H

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include <climits>
#include <iosfwd>
  
class Envelope
{
    public:
        virtual double getAmplitude(double timeIndexSeconds) = 0;
};
 
#endif
