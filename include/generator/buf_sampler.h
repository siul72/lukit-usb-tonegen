#ifndef BUF_SAMPLER_H
#define BUF_SAMPLER_H

#include "generator/sampler.h"
#include "generator/tonegen.h"
 
class BufSampler: public Sampler
{
    
    public:
        BufSampler(int sampleRateHz, int bitsPerSample, int numChannels, double volume, int duration_milliseconds);
        void sample();
        SamplerType getType() const;
        void setSampler(ToneGenerator *generator, int toneFrequencyHz, Envelope *envelope);
        void getSample(char **buf, uint32_t *cur_len);
        std::vector<char>& getSampleData();
        
};
 
#endif
