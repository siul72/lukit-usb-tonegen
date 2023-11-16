#include "generator/sampler.h"

 
Sampler::Sampler(int sampleRateHz, int bitsPerSample, int numChannels, double in_volume, double durationMilliseconds){
    if(numChannels != 1){
           return;
    }
   
    if(bitsPerSample != 8){
        return;
    }
    
    this->bits_per_sample = bitsPerSample;
    this->sample_value_range = pow(2, this->bits_per_sample);
    this->sample_duration_time = durationMilliseconds;
    this->num_channels = numChannels;
    this->sample_rate_hz = sampleRateHz;
    
    this->volume = in_volume;
    this->rd_ptr = 0;

    this->usb_in_len = 4 * sample_rate_hz / 1000;
 }

void Sampler::setSampler(ToneGenerator *generator, int toneFrequencyHz, Envelope *envelope){
    this->generator = generator;
    this->tone_frequency_hz = toneFrequencyHz;
    this->envelope = envelope;
    this->sample_buffer_size = sample_rate_hz * num_channels * bits_per_sample/8 * sample_duration_time/1000;
}

void Sampler::setVolume(double in_volume){
     if(in_volume < 0 || in_volume > 1) {
        // loudest
       this->volume = 1.0;
      
    } 

    this->volume = in_volume;
    
}

int Sampler::getSampleRateHz(){
    return this->sample_rate_hz;
}

int Sampler::getBitsPerSample(){
    return this->bits_per_sample;
}

int Sampler::getNumChannels(){
    return this->num_channels;
}
 

 