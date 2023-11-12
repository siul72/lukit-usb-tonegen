#include "generator/buf_sampler.h"
#include "generator/envelope.h"
#include "generator/tonegen.h" 
 
BufSampler::BufSampler(int sampleRateHz, int bitsPerSample, int numChannels): Sampler(sampleRateHz, bitsPerSample, numChannels){
 
}

 
void BufSampler::sample(ToneGenerator* generator, int toneFrequencyHz, double durationSeconds, Envelope* envelope, double volume)
{
    if(volume == 11) {
        // loudest
        volume = 1.0;
    } 
    
    if(volume < 0 || volume > 1) {
        //throw std::logic_error("Invalid volume: must be within range 0.0 .. 1.0");
        volume = 1.0;
        return;
    }
       

    const double sampleValueRange = pow(2, this->bitsPerSample);
    const int sample_buffer_size = this->sampleRateHz * durationSeconds;
    //sprintf(Utils::getInstance()->UART2_TX_Buffer, "Generate tone number of samples = %d\n", (int)sample_buffer_size);
    //Logging::getInstance()->console_write(Utils::getInstance()->UART2_TX_Buffer);
    for(int i=0; i < sample_buffer_size; i++) {
        double timeIndexSeconds = (double)i / this->sampleRateHz;
        double sample = generator->generate(toneFrequencyHz, timeIndexSeconds, durationSeconds);
         // apply envelope
        sample = sample * envelope->getAmplitude(timeIndexSeconds);
        // apply volume
        sample = sample * volume;
        // map continous result from tone generator [-1.0, 1.0] to discrete sample value range [0 .. 255]
        char sampleValue = sampleValueRange * (sample + 1.0) / 2.0;
        sampleValue = sampleValue + 127;
        this->sampleData.push_back(0);
        this->sampleData.push_back(sampleValue);
        this->sampleData.push_back(0);
        this->sampleData.push_back(sampleValue);
    }
}


std::vector<char> &BufSampler::getSampleData(){
    return this->sampleData;
}
 