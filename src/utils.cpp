/**********************************************************************************
* File: utils.cpp
* Author: Luis Coelho <luis.coelho.720813@gmail.com>
* Date: 2023.11.11
* Description: Main File for the Luki Generator
*
* "The best way to predict the future is to write code and create it" - David Heinemeier Hansson
*
* Copyright (c) 2023 Luis Coelho
* This code is licensed under the CC-BY-ND license.
 
* Tone Generator © 2023 by Luis Coelho is licensed under Attribution-NoDerivatives 4.0 International. 
* To view a copy of this license, visit http://creativecommons.org/licenses/by-nd/4.0/
* You are free to:
* - Share — copy and redistribute the material in any medium or format
* - Attribute — You must give appropriate credit, provide a link to the license, and indicate if changes were made.
*
* Under the following terms:
* - NoDerivatives — If you remix, transform, or build upon the material, you may not distribute the
* modified material.
*
* THE WORK IS PROVIDED "AS IS", WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING, WITHOUT LIMITATION, ANY WARRANTIES OR CONDITIONS OF TITLE, NON-INFRINGEMENT, MERCHANTABILITY,
* OR FITNESS FOR A PARTICULAR PURPOSE. YOU ARE SOLELY RESPONSIBLE FOR DETERMINING THE APPROPRIATENESS OF
* USING OR REDISTRIBUTING THE WORK AND ASSUME ANY RISKS ASSOCIATED WITH YOUR USE OF THIS WORK.
***********************************************************************************/
#include <math.h>
#include "utils.h"
#include "usb/usbd_audio.h"
#include "logging.h"
 
extern ADC_HandleTypeDef hadc1;
extern USBD_HandleTypeDef hUsbDeviceFS;
 
int16_t adc_buffer[OSA_BUF_SIZE * AUDIO_IN_PACKET / 2] = {0};
 
Utils* Utils::getInstance()  {
    if (instancePtr == NULL) {
         instancePtr = new Utils(); 
           return instancePtr; 
    }
    else{
       return instancePtr;
    }
}

Utils::Utils(){
  
  this->init();
}

void Utils::init(){
    const int sampleRateHz    = 48000;    // number of samples per second
    const int numChannels     = 1;        // Mono
    const int bitsPerSample   = 8; // 8 bits

    noteDuration = 50;
    pureTone = new PureToneGenerator();
    sampler = new BufSampler(sampleRateHz, bitsPerSample, numChannels, volume, noteDuration);
    noEnvelope = new NoEnvelope();

    sampler->setSampler(pureTone, A4, noEnvelope);
    sampler->sample();
    rd_ptr = 0;
    usb_in_len = 4 * sampleRateHz / 1000;
 
    tone_buffer_len = sizeof(char)*sampler->getSampleData().size();
    sprintf(Logging::UART2_TX_Buffer, "sampler %d bytes\n", (int)tone_buffer_len);
    Logging::getInstance()->console_write(Logging::UART2_TX_Buffer);

}
 

void Utils::error_handler(void){
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
 

void Utils::get_sine_sample(char **buf, u_int32_t * cur_len){

    *cur_len = usb_in_len;
    *buf = (char *)&sampler->getSampleData()[rd_ptr];
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

uint32_t Utils::usb_audio_write_tx_data(char* buf, uint32_t len){
   
     return len;
}

void Utils::ADC_to_MIC(void){
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_buffer, OSA_BUF_SIZE * (AUDIO_IN_PACKET / 2)); // Start ADC transfer into oversampling buffer
}
 

