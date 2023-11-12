#ifndef SAMPLER_H
#define SAMPLER_H
 
#include <vector>
#include "tonegen.h" 
#include "envelope.h"
 
class Sampler
{
    protected:
        int sampleRateHz;
        int bitsPerSample;
        int numChannels;
        std::vector<char> sampleData;
       
    public:
    
        Sampler(int sampleRateHz, int bitsPerSample, int numChannels);
        virtual void sample(ToneGenerator* generator, int toneFrequencyHz, double durationSeconds, Envelope* envelope, double volume) = 0;
        virtual std::vector<char>& getSampleData() = 0;
        int getSampleRateHz();
        int getBitsPerSample();
        int getNumChannels();
};
 
#endif
