#include "generator/violin_tonegen.h"
 

// Violin sound https://meettechniek.info/additional/additive-synthesis.html
double ViolinGenerator::generate(int fundamentalFrequencyHz, double timeIndexSeconds, double durationSeconds)
{
    UNUSED(durationSeconds);
    
    double amplitude = 0.49;

    double harmonic1PeriodSeconds = 1.0 / fundamentalFrequencyHz;
    double harmonic1Radians = timeIndexSeconds / harmonic1PeriodSeconds * (2 * M_PI);
    double harmonic1Amplitude = 0.995;

    double harmonic2PeriodSeconds = 1.0 / (fundamentalFrequencyHz * 2);
    double harmonic2Radians = timeIndexSeconds / harmonic2PeriodSeconds * (2 * M_PI);
    double harmonic2Amplitude = 0.940;

    double harmonic3PeriodSeconds = 1.0 / (fundamentalFrequencyHz * 3);
    double harmonic3Radians = timeIndexSeconds / harmonic3PeriodSeconds * (2 * M_PI);
    double harmonic3Amplitude = 0.425;

    double harmonic4PeriodSeconds = 1.0 / (fundamentalFrequencyHz * 4);
    double harmonic4Radians = timeIndexSeconds / harmonic4PeriodSeconds * (2 * M_PI);
    double harmonic4Amplitude = 0.480;

    double harmonic6PeriodSeconds = 1.0 / (fundamentalFrequencyHz * 6);
    double harmonic6Radians = timeIndexSeconds / harmonic4PeriodSeconds * (2 * M_PI);
    double harmonic6Amplitude = 0.365;

    double harmonic7PeriodSeconds = 1.0 / (fundamentalFrequencyHz * 7);
    double harmonic7Radians = timeIndexSeconds / harmonic7PeriodSeconds * (2 * M_PI);
    double harmonic7Amplitude = 0.040;

    double harmonic8PeriodSeconds = 1.0 / (fundamentalFrequencyHz * 8);
    double harmonic8Radians = timeIndexSeconds / harmonic8PeriodSeconds * (2 * M_PI);
    double harmonic8Amplitude = 0.085;

    double harmonic10PeriodSeconds = 1.0 / (fundamentalFrequencyHz * 10);
    double harmonic10Radians = timeIndexSeconds / harmonic10PeriodSeconds * (2 * M_PI);
    double harmonic10Amplitude = 0.090;
 
    double result = amplitude *
                  ( harmonic1Amplitude  * sin(harmonic1Radians)
                  + harmonic2Amplitude  * cos(harmonic2Radians)
                  + harmonic3Amplitude  * sin(harmonic3Radians)
                  + harmonic4Amplitude  * cos(harmonic4Radians)
                  + harmonic6Amplitude  * cos(harmonic6Radians)
                  + harmonic7Amplitude  * sin(harmonic6Radians)
                  + harmonic8Amplitude  * cos(harmonic8Radians)
                  + harmonic10Amplitude * cos(harmonic10Radians) );

    return result;
}

GeneratorType ViolinGenerator::getType() const  {
        return GeneratorType::Violin;
}
 