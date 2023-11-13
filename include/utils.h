#ifndef __UTILS__H__
#define __UTILS__H__


#include <stdio.h>
#include <string.h>
#include <usbd_def.h> 
#include "generator/buf_sampler.h"
#include "generator/pure_tonegen.h"
#include "generator/no_envelope.h"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0') 

class Utils {

public:
    static Utils* getInstance();
    Utils();
    Utils(const Utils& obj) = delete;
    void init();
  
    static void error_handler(void);
   
    void get_sine_sample(char ** buf, u_int32_t *len);
    uint32_t usb_audio_write_tx_data(char *buf, uint32_t len);
    void ADC_to_MIC(void);
    uint8_t USBD_AUDIO_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum);
    static UART_HandleTypeDef huart2;
    static uint8_t UART2_RX_Buffer[26];
    static char UART2_TX_Buffer[128];
    DMA_HandleTypeDef hdma_usart2_rx;
    uint8_t audioBufferTx[1024];

private:
    
    static Utils* instancePtr; 
    int32_t sample_period;
    double theta;
    uint8_t val;
    uint16_t sample_rate = 8000;
    int sin_hz = 200;
    int step = 50;
    int bit_rate = 8;
    u_int32_t sample_byte_len;
    unsigned long last_micros;
    double inc;
    BufSampler *sampler;
    PureToneGenerator *pureTone;
    NoEnvelope *noEnvelope;
    const double volume       = 0.75;     // 0.0 .. 1.0
    double noteDuration;     // seconds
    uint32_t rd_ptr;
    uint32_t usb_in_len;
    uint32_t tone_buffer_len;


};

#endif