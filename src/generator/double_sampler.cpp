#include "generator/double_sampler.h"
#include "logging.h"
 
 

DoubleSampler::DoubleSampler(int sampleRateHz, int bitsPerSample, int numChannels, double volume = 0.75, int durationMilliSeconds=1): Sampler(sampleRateHz, bitsPerSample, numChannels, volume, durationMilliSeconds) {
 
    for(uint8_t i = 0; i < DOUBLE_BUF_SIZE; i++){
        this->buf_status[i]=BUF_STATUS_FREE;
    }

    timeIndexSeconds = 0;
}
 
void DoubleSampler::sample(){

    uint8_t buf_index = 0xff;
    //check available buffer
    for(uint8_t i = 0; i < DOUBLE_BUF_SIZE; i++){
        if (this->buf_status[i] == BUF_STATUS_FREE){
            buf_index = i;
            break;
        }
    }

    if (buf_index == 0xff){
        return;
    }

    //sprintf(Logging::UART2_TX_Buffer, "write buf_index=%u", buf_index);
    //Logging::getInstance()->console_write(Logging::UART2_TX_Buffer);

    this->buf_status[buf_index] = BUF_STATUS_WRITING;
    //sample
    this->internal_buffer[buf_index].clear();
    for(uint32_t i=0; i < sample_buffer_size; i++) {
        timeIndexSeconds = timeIndexSeconds + (double)i / this->sample_rate_hz;
        double sample = generator->generate(tone_frequency_hz, timeIndexSeconds, sample_duration_time);
         // apply envelope
        sample = sample * envelope->getAmplitude(timeIndexSeconds);
        // apply volume
        sample = sample * volume;
        // map continous result from tone generator [-1.0, 1.0] to discrete sample value range [0 .. 255]
        char sampleValue = (double)sample_value_range/2.0 * (sample + 1.0);
        //char16_t sampleValue = (char16_t)v;
        sampleValue = sampleValue + 127;
        this->internal_buffer[buf_index].push_back(0);
        this->internal_buffer[buf_index].push_back(sampleValue);
        this->internal_buffer[buf_index].push_back(0);
        this->internal_buffer[buf_index].push_back(sampleValue);
    }

    this->buf_status[buf_index] = BUF_STATUS_READ_READY;

}

SamplerType DoubleSampler::getType() const  {
        return SamplerType::DoubleBuffer;
}

void DoubleSampler::getSample(char **buf, uint32_t * cur_len) {

    std::vector<char>  v = this->getSampleData();
    *buf = (char *) &v;
    *cur_len = v.size();

}

std::vector<char> &DoubleSampler::getSampleData(){
    
    uint8_t buf_index = 0xff;
    for(uint8_t i = 0; i < DOUBLE_BUF_SIZE; i++){
      
        if (this->buf_status[i] == BUF_STATUS_READING){
            this->buf_status[i] = BUF_STATUS_FREE;
        }
    }

    for(uint8_t i = 0; i < DOUBLE_BUF_SIZE; i++){
        if (this->buf_status[i] == BUF_STATUS_READ_READY){
            buf_index = i;
            break;
        }
 
    }

    if (buf_index == 0xff){
        return this->sample_data;
    }

    //sprintf(Logging::UART2_TX_Buffer, "read buf_index=%u", buf_index);
    //Logging::getInstance()->console_write(Logging::UART2_TX_Buffer);

    this->buf_status[buf_index] = BUF_STATUS_READING;
    return this->internal_buffer[buf_index];
    
}
 
