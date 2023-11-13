#ifndef BUF_SAMPLER_H
#define BUF_SAMPLER_H

#include "generator/sampler.h"
#include "generator/tonegen.h"
 
class DoubleSampler: public Sampler
{
    private:
        
        DoubleSampler();
        std::vector<char> internal_buffer[2];
        
        int current_wr_buffer_index;
        int current_rd_buffer_index;

    public:
 
        DoubleSampler(int sampleRateHz, int bitsPerSample, int numChannels, double volume, int durationMilliSeconds);
   
        void sample();

        std::vector<char>& getSampleData();  
};
 
#endif
