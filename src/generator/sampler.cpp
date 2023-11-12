#include "generator/sampler.h"
 
 
Sampler::Sampler(int sampleRateHz, int bitsPerSample, int numChannels){
    if(numChannels != 1)
    //throw std::logic_error("Unsupported value for numChannels: only 1 channel (mono) supported");
    return;

    if(bitsPerSample != 8)
    //throw std::logic_error("Unsupported value for bitsPerSample: only 8 bits supported");
    return;
 }


int Sampler::getSampleRateHz(){
    return this->sampleRateHz;
}

int Sampler::getBitsPerSample(){
    return this->bitsPerSample;
}

int Sampler::getNumChannels(){
    return this->numChannels;
}

 