#include "generator/buf_sampler.h"
#include "generator/envelope.h"
#include "generator/tonegen.h" 
#include "logging.h"
 
 
 
BufSampler::BufSampler(int sampleRateHz, int bitsPerSample, int numChannels, double volume=0.75, int duration_milliseconds=1): 
Sampler(sampleRateHz, bitsPerSample, numChannels, volume, duration_milliseconds){

    
 
}

void BufSampler::setSampler(ToneGenerator *generator, int toneFrequencyHz, Envelope *envelope){
    Sampler::setSampler(generator, toneFrequencyHz, envelope);
    this->sample_duration_time = 1 / (double)tone_frequency_hz;
    this->sample_buffer_size = sample_rate_hz * num_channels * bits_per_sample/8 * sample_duration_time;

}

SamplerType BufSampler::getType() const  {
        return SamplerType::Buffer;
}

 
void BufSampler::sample()
{
    
    //sprintf(Utils::getInstance()->UART2_TX_Buffer, "Generate tone number of samples = %d\n", (int)sample_buffer_size);
    //Logging::getInstance()->console_write(Utils::getInstance()->UART2_TX_Buffer);
    this->sample_data.clear();
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

void BufSampler::getSample(char **buf, uint32_t * cur_len){

    *cur_len = usb_in_len;
    *buf = (char *)&this->getSampleData()[rd_ptr];
    tone_buffer_len = this->getSampleData().size();
    rd_ptr = rd_ptr + *cur_len;
    if (rd_ptr >= tone_buffer_len){
      
      *cur_len =  usb_in_len - (rd_ptr - tone_buffer_len);
      //sprintf(this->logger.UART2_TX_Buffer, "short %d:%d:%d:%d\n", (int)tone_buffer_len, (int)cur_len, (int)usb_in_len, (int)rd_ptr);
      //this->console_write(this->logger.UART2_TX_Buffer);
      rd_ptr = 0;
      if (*cur_len > usb_in_len){
          Logging::getInstance()->console_write("there is a bug!");   
      }
    }
    
    //USBD_HandleTypeDef *pdev = &hUsbDeviceFS;
    //USBD_LL_Transmit(pdev, AUDIO_MIC_IN_USB_EP, (uint8_t*)(buf), cur_len);
    //sprintf(this->logger.UART2_TX_Buffer, "tx %d bytes at %d\n", (int)usb_in_len, (int)rd_ptr);
    //this->console_write(this->logger.UART2_TX_Buffer);
  
}


std::vector<char> &BufSampler::getSampleData(){
    return this->sample_data;
}
 
 