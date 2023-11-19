#include "generator/double_sampler.h"
#include "logging.h"
 
 

DoubleSampler::DoubleSampler(int sampleRateHz, int bitsPerSample, int numChannels, double volume = 0.75, int durationMilliSeconds=1): Sampler(sampleRateHz, bitsPerSample, numChannels, volume, durationMilliSeconds) {
 
    for(uint8_t i = 0; i < DOUBLE_BUF_SIZE; i++){
        this->buf_status[i]=BUF_STATUS_FREE;
    }
        

    current_time_seconds = 0;
    current_time_index=0;
    buf_index = 0;
}
 
void DoubleSampler::sample(){
 
    if (this->buf_status[buf_index] == BUF_STATUS_WRITING){
        return;
    }

    this->buf_status[buf_index] = BUF_STATUS_WRITING;
    this->internal_buffer[buf_index].clear();
    for(uint32_t i=0; i < sample_buffer_size; i++) {
        
        current_time_seconds =  (double)(i+current_time_index) / this->sample_rate_hz;
        double sample = this->generator->generate(tone_frequency_hz, current_time_seconds, sample_duration_time);
        sample = sample * this->envelope->getAmplitude(current_time_seconds);
        sample = sample * this->volume;
        char sampleValue = (double)sample_value_range/2.0 * (sample + 1.0);
        sampleValue = sampleValue + 127;
        this->internal_buffer[buf_index].push_back(0);
        this->internal_buffer[buf_index].push_back(sampleValue);
        this->internal_buffer[buf_index].push_back(0);
        this->internal_buffer[buf_index].push_back(sampleValue);
    }
    current_time_index = current_time_index  + sample_buffer_size;
    this->buf_status[buf_index] = BUF_STATUS_READ_READY;
    buf_index = 1 - buf_index;

}

SamplerType DoubleSampler::getType() const  {
        return SamplerType::DoubleBuffer;
}

void DoubleSampler::getSample(char **buf, uint32_t * cur_len) {
   
    sprintf(Logging::UART2_TX_Buffer, "getSample buf_index=%u ptr=%u\n", buf_index, (uint)rd_ptr);
    Logging::getInstance()->console_write(Logging::UART2_TX_Buffer);
     
    if (rd_ptr == 0){
        current_read_buffer = this->getSampleData();
    }
    *cur_len = usb_in_len;
    *buf = (char *)&current_read_buffer[rd_ptr];
    rd_ptr = rd_ptr + *cur_len;
    tone_buffer_len = current_read_buffer.size();
     
    if (rd_ptr >= tone_buffer_len){
      this->sample();
      *cur_len =  usb_in_len - (rd_ptr - tone_buffer_len);
      rd_ptr = 0;
      if (*cur_len > usb_in_len){
          Logging::getInstance()->console_write("there is a bug!");   
      }
    }
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

    if (buf_index == 255){
        Logging::getInstance()->console_write("no read buffer this time!");
        return this->sample_data;    
    }
    this->buf_status[buf_index] = BUF_STATUS_READING;
    return this->internal_buffer[buf_index];
    
}
 
