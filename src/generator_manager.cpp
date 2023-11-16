/**********************************************************************************
* File: generator_manager.cpp
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
#include "generator_manager.h"
#include "usb/usbd_audio.h"
#include "logging.h"
 
extern ADC_HandleTypeDef hadc1;
extern USBD_HandleTypeDef hUsbDeviceFS;
 
int16_t adc_buffer[OSA_BUF_SIZE * AUDIO_IN_PACKET / 2] = {0};
 
GeneratorManager* GeneratorManager::getInstance()  {
    if (instancePtr == NULL) {
         instancePtr = new GeneratorManager(); 
           return instancePtr; 
    }
    else{
       return instancePtr;
    }
}

GeneratorManager::GeneratorManager(){
    this->init();
}

void GeneratorManager::sample(){
   //only fetch samples for double sampler
   if (sampler->getType() == SamplerType::DoubleBuffer){
      sampler->sample();
   }
}

void GeneratorManager::init(){
    const int sampleRateHz    = 48000;    // number of samples per second
    const int numChannels     = 1;        // Mono
    const int bitsPerSample   = 8; // 8 bits

    noteDuration = 1;
    pureTone = new PureToneGenerator();
    //sampler = new BufSampler(sampleRateHz, bitsPerSample, numChannels, volume, noteDuration);
    sampler = new DoubleSampler(sampleRateHz, bitsPerSample, numChannels, volume, noteDuration);
    noEnvelope = new NoEnvelope();
    sampler->setSampler(pureTone, A4, noEnvelope);
    sampler->sample();

 
    //tone_buffer_len = sizeof(char)*sampler->getSampleData().size();
    //sprintf(Logging::UART2_TX_Buffer, "sampler %d bytes\n", (int)tone_buffer_len);
    //Logging::getInstance()->console_write(Logging::UART2_TX_Buffer);

}
 
void GeneratorManager::getSample(char **buf, uint32_t * cur_len){
    this->sampler->getSample(buf, cur_len);
  
}

uint32_t GeneratorManager::usb_audio_write_tx_data(char* buf, uint32_t len){
   
     return len;
}

void GeneratorManager::ADC_to_MIC(void){
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_buffer, OSA_BUF_SIZE * (AUDIO_IN_PACKET / 2)); // Start ADC transfer into oversampling buffer
}

void GeneratorManager::error_handler(void){
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
 

