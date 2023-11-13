#include "generator/buf_sampler.h"
#include "generator/envelope.h"
#include "generator/tonegen.h" 
 
 
BufSampler::BufSampler(int sampleRateHz, int bitsPerSample, int numChannels, double volume=0.75, int duration_milliseconds=1): 
Sampler(sampleRateHz, bitsPerSample, numChannels, volume, duration_milliseconds){
 
}

 
void BufSampler::sample()
{
    
    //sprintf(Utils::getInstance()->UART2_TX_Buffer, "Generate tone number of samples = %d\n", (int)sample_buffer_size);
    //Logging::getInstance()->console_write(Utils::getInstance()->UART2_TX_Buffer);
    for(uint32_t i=0; i < sample_buffer_size; i++) {
        double timeIndexSeconds = (double)i / this->sample_rate_hz;
        double sample = generator->generate(tone_frequency_hz, timeIndexSeconds, sample_duration_time / 1000);
         // apply envelope
        sample = sample * envelope->getAmplitude(timeIndexSeconds);
        // apply volume
        sample = sample * volume;
        // map continous result from tone generator [-1.0, 1.0] to discrete sample value range [0 .. 255]
        char sampleValue = sample_value_range * (sample + 1.0) / 2.0;
        sampleValue = sampleValue + 127;
        this->sample_data.push_back(0);
        this->sample_data.push_back(sampleValue);
        this->sample_data.push_back(0);
        this->sample_data.push_back(sampleValue);
    }
}


std::vector<char> &BufSampler::getSampleData(){
    return this->sample_data;
}
 