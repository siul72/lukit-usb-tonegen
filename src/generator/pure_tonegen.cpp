#include "generator/pure_tonegen.h"
  
double PureToneGenerator::generate(int toneFrequencyHz, double timeIndexSeconds, double durationSeconds)
{
    
    UNUSED(durationSeconds);
    double tonePeriodSeconds = 1.0 / toneFrequencyHz;
    double radians = timeIndexSeconds / tonePeriodSeconds * (2 * M_PI);
    double result = sin(radians);

    return result;
}

GeneratorType PureToneGenerator::getType() const  {
        return GeneratorType::Pure;
}
 
