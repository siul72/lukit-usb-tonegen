#include "generator/sampler.h"

 
Sampler::Sampler(int sampleRateHz, int bitsPerSample, int numChannels, double in_volume, int durationMilliseconds){
    if(numChannels != 1){
           return;
    }
   
    if(bitsPerSample != 8){
        return;
    }
    
    sample_value_range = pow(2, this->bits_per_sample);
    sample_duration_time = durationMilliseconds;
    sample_buffer_size = this->sample_rate_hz * durationMilliseconds;
    volume = in_volume;
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

 