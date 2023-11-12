#ifndef SAMPLER_H
#define SAMPLER_H
 
#include <vector>
#include "tonegen.h" 
#include "envelope.h"
 
class Sampler
{
    protected:
        int sample_rate_hz;
        int bits_per_sample;
        int num_channels;
        int sample_value_range;
        int sample_buffer_size;
        int sample_duration_time;
        std::vector<char> sample_data;
        double volume;
       
    public:
    
        Sampler(int sampleRateHz, int bitsPerSample, int numChannels, double volume, int durationMilliseconds);
        void setVolume(double in_volume);
        virtual void sample(ToneGenerator *generator, int toneFrequencyHz, Envelope *envelope) = 0;
        virtual std::vector<char>& getSampleData() = 0;
        int getSampleRateHz();
        int getBitsPerSample();
        int getNumChannels();
};
 
#endif
