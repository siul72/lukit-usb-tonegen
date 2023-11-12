#ifndef SAMPLER_H
#define SAMPLER_H

#include <vector>
#include "tonegen.h" 
#include "envelope.h"
 
class Sampler
{
    private:
        int sampleRateHz;
        int bitsPerSample;
        int numChannels;
        std::vector<char> sampleData;
        Sampler();
    public:
        Sampler(int sampleRateHz, int bitsPerSample, int numChannels);
        void sample(ToneGenerator* generator, int toneFrequencyHz, double durationSeconds, Envelope* envelope, double volume);
        int getSampleRateHz();
        int getBitsPerSample();
        int getNumChannels();
        std::vector<char>& getSampleData(); // TODO should consider returning "const" value
};
 
#endif
