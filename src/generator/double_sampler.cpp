#include "generator/double_sampler.h"
 

DoubleSampler::DoubleSampler(int sampleRateHz, int bitsPerSample, int numChannels, double volume = 0.75, int durationMilliSeconds=1): Sampler(sampleRateHz, bitsPerSample, numChannels, volume, durationMilliSeconds) {

    active_buffer = 0;

}

void DoubleSampler::sample(ToneGenerator *generator, int toneFrequencyHz, Envelope *envelope){

    //check available buffer

    //sample
    for(int i=0; i < sample_buffer_size; i++) {
        double timeIndexSeconds = (double)i / this->sample_rate_hz;
        double sample = generator->generate(toneFrequencyHz, timeIndexSeconds, sample_duration_time);
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

std::vector<char> &DoubleSampler::getSampleData(){
    // TODO: insert return statement here
    return this->sample_data;
}
