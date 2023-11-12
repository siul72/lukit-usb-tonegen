 #include "generator/adsr_envelope.h"

ADSREnvelope::ADSREnvelope(double durationSeconds)
{
    if(durationSeconds <= 0.0)
        //std::logic_error("Invalid value for durationSeconds: must be positive non-zero value");
        return;

    this->attackAmplitude       = 1.0;
    this->attackDurationSeconds = durationSeconds * 0.1;

    // decay amplitude range is given by attack and sustain
    this->decayDurationSeconds = durationSeconds * 0.1;

    this->sustainAmplitude       = 0.7;
    this->sustainDurationSeconds = durationSeconds * 0.7;

    this->releaseAmplitude       = 0.0;
    this->releaseDurationSeconds = durationSeconds * 0.1;
}

double ADSREnvelope::getAmplitude(double timeIndexSeconds)
{
    double result;

    if(timeIndexSeconds < this->attackDurationSeconds)
    {
        // linearly increasing
        result = this->attackAmplitude / this->attackDurationSeconds * timeIndexSeconds;
    }
    else if(timeIndexSeconds < this->attackDurationSeconds + this->decayDurationSeconds)
    {
        double relativeTimeIndexSeconds = timeIndexSeconds - this->attackDurationSeconds;
        double relativeAmplitude = this->attackAmplitude - this->sustainAmplitude;

        // linearly decreasing
        result = this->attackAmplitude - (relativeAmplitude / this->decayDurationSeconds * relativeTimeIndexSeconds);
    }
    else if(timeIndexSeconds < this->attackDurationSeconds + this->decayDurationSeconds + this->sustainDurationSeconds)
    {
        double relativeTimeIndexSeconds = timeIndexSeconds - this->attackDurationSeconds - this->decayDurationSeconds;

        // keep at same level
        result = this->sustainAmplitude;
    }
    else
    {
        double relativeTimeIndexSeconds = timeIndexSeconds - this->attackDurationSeconds - this->decayDurationSeconds - this->sustainDurationSeconds;

        // linearly decreasing
        result = this->sustainAmplitude - (this->sustainAmplitude / this->releaseDurationSeconds * relativeTimeIndexSeconds);
    }

    return result;
}
 