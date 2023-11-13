#ifndef SAMPLER_H
#define SAMPLER_H
 
#include <vector>
#include "tonegen.h" 
#include "envelope.h"
 
class Sampler
{
    protected:
        uint32_t sample_rate_hz;
        int8_t bits_per_sample;
        uint8_t num_channels;
        uint32_t sample_value_range;
        uint32_t sample_buffer_size;
        uint32_t sample_duration_time;
        std::vector<char> sample_data;
        double volume;
        ToneGenerator *generator;
        uint32_t tone_frequency_hz;
        Envelope *envelope;
       
    public:
    
        Sampler(int sampleRateHz, int bitsPerSample, int numChannels, double volume, int durationMilliseconds);
        void setSampler(ToneGenerator *generator, int toneFrequencyHz, Envelope *envelope);
        void setVolume(double in_volume);
        int getSampleRateHz();
        int getBitsPerSample();
        int getNumChannels();
        virtual void sample() = 0;
        virtual std::vector<char>& getSampleData() = 0;
       
};
 
#endif
