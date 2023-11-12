#ifndef BUF_SAMPLER_H
#define BUF_SAMPLER_H

#include "generator/sampler.h"
#include "generator/tonegen.h"
 
class BufSampler: public Sampler
{
    private:
        
        BufSampler();
    public:
        BufSampler(int sampleRateHz, int bitsPerSample, int numChannels);
        void sample(ToneGenerator* generator, int toneFrequencyHz, double durationSeconds, Envelope* envelope, double volume);
        int getSampleRateHz();
        int getBitsPerSample();
        int getNumChannels();
        std::vector<char>& getSampleData();  
};
 
#endif
