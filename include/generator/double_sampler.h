#ifndef DOUBLE_SAMPLER_H
#define DOUBLE_SAMPLER_H

#include "generator/sampler.h"
#include "generator/tonegen.h"

#define DOUBLE_BUF_SIZE 2

enum buf_status_enum {
 
    BUF_STATUS_NONE=1,
    BUF_STATUS_FREE=0,
    BUF_STATUS_WRITING=1,
    BUF_STATUS_READ_READY=2,
    BUF_STATUS_READING=3
};
 
class DoubleSampler: public Sampler
{
    private:
        
        DoubleSampler();
        std::vector<char> internal_buffer[DOUBLE_BUF_SIZE];
        
        int current_wr_buffer_index;
        int current_rd_buffer_index;
        buf_status_enum buf_status[DOUBLE_BUF_SIZE];

    public:
 
        DoubleSampler(int sampleRateHz, int bitsPerSample, int numChannels, double volume, int durationMilliSeconds);
        void sample();
        std::vector<char>& getSampleData();  
        void getSample(char **buf, uint32_t * cur_len);
};
 
#endif
