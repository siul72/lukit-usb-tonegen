#ifndef SAMPLER_H
#define SAMPLER_H
 
#include <vector>
#include "tonegen.h" 
#include "envelope.h"

enum class SamplerType {
    Buffer,
    DoubleBuffer
};
 
class Sampler
{
    protected:
        uint32_t sample_rate_hz;
        int8_t bits_per_sample;
        uint8_t num_channels;
        uint32_t sample_value_range;
        uint32_t sample_buffer_size;
        double sample_duration_time;
        std::vector<char> sample_data;
        double volume;
        ToneGenerator *generator;
        uint32_t tone_frequency_hz;
        Envelope *envelope;
        uint32_t rd_ptr;
        uint32_t usb_in_len;
        uint32_t tone_buffer_len;
       
    public:
    
        Sampler(int sampleRateHz, int bitsPerSample, int numChannels, double volume, double durationMilliseconds);
        virtual void setSampler(ToneGenerator *generator, int toneFrequencyHz, Envelope *envelope);
        virtual SamplerType getType() const = 0;
        void setVolume(double in_volume);
        int getSampleRateHz();
        int getBitsPerSample();
        int getNumChannels();
        virtual void sample() = 0;
        virtual std::vector<char>& getSampleData() = 0;
        virtual void getSample(char ** buf, uint32_t *len) = 0;
      
       
};
 
#endif
