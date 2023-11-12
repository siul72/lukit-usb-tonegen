 #include "generator/chirp_tonegen.h"
 
double ChirpGenerator::generate(int initialFrequencyHz, double timeIndexSeconds, double durationSeconds)
{
    int finalFrequencyHz = initialFrequencyHz * 10;

    // modulate the frequency with time, linearly decreasing from initialFrequencyHz to finalFrequencyHz
    int momentaryFrequencyHz = initialFrequencyHz + (double)finalFrequencyHz / durationSeconds * timeIndexSeconds;

    double tonePeriodSeconds = 1.0 / momentaryFrequencyHz;
    double radians = timeIndexSeconds / tonePeriodSeconds * (2 * M_PI);
    double result = sin(radians);

    return result;
}
 