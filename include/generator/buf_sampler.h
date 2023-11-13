#ifndef BUF_SAMPLER_H
#define BUF_SAMPLER_H

#include "generator/sampler.h"
#include "generator/tonegen.h"
 
class BufSampler: public Sampler
{
    
    public:
        BufSampler(int sampleRateHz, int bitsPerSample, int numChannels, double volume, int duration_milliseconds);
        void sample();
  
        std::vector<char>& getSampleData();  
};
 
#endif
