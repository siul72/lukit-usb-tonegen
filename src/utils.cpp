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
#include "utils.h"
#include <math.h>
#include <usbd_audio.h>

extern int32_t log_counter;
extern ADC_HandleTypeDef hadc1;
extern USBD_HandleTypeDef hUsbDeviceFS;
 
UART_HandleTypeDef Utils::huart2;
uint8_t Utils::UART2_RX_Buffer[26];
char Utils::UART2_TX_Buffer[128];

int16_t adc_buffer[OSA_BUF_SIZE * AUDIO_IN_PACKET / 2] = {0};
 
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  
    sprintf(Utils::UART2_TX_Buffer, "Roger, got 26 chars, got for next 26!\n");
    HAL_UART_Transmit(&Utils::huart2, (uint8_t *)Utils::UART2_TX_Buffer, strlen(Utils::UART2_TX_Buffer), 10);
    HAL_UART_Transmit(&Utils::huart2, Utils::UART2_RX_Buffer, 26, 10);
    HAL_UART_Receive_DMA(&Utils::huart2, Utils::UART2_RX_Buffer, 26);
}

Utils* Utils::getInstance()  {
    
    if (instancePtr == NULL) 
    {
         instancePtr = new Utils(); 
           return instancePtr; 
    }
    else
    {
       
      return instancePtr;
    }
  }

Utils::Utils()
{
  this->init_uart();
  this->init();
}

void Utils::init()
{
    
    const int sampleRateHz    = 48000;    // number of samples per second
    const int numChannels     = 1;        // Mono
    const int bitsPerSample   = CHAR_BIT; // 8 bits

    noteDuration = 0.001;
    pureTone = new PureToneGenerator();
    sampler = new Sampler(sampleRateHz, bitsPerSample, numChannels);
    noEnvelope = new NoEnvelope();

    sampler->sample(pureTone, C2, noteDuration, noEnvelope, volume);

}

void Utils::init_uart()
{
  
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 234000;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    error_handler();
  }

  HAL_UART_Receive_DMA(&huart2, UART2_RX_Buffer, 26);
  
 
}
 


void Utils::error_handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

void Utils::console_write(const char* msg)
{
    HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), 10);
     
}

double Utils::get_increment_from_hz(int hz)
{
  return (2 * M_PI) / ((1000000.0 / hz) / sample_period);
}

void Utils::get_sine_sample(){
   
    char* buf = (char *)&sampler->getSampleData()[0];
    uint32_t len = sizeof(char)*sampler->getSampleData().size();
    USBD_HandleTypeDef *pdev = &hUsbDeviceFS;
    USBD_LL_Transmit(pdev, AUDIO_MIC_IN_USB_EP, (uint8_t*)(buf), len);
    sprintf(this->UART2_TX_Buffer, "tx %d bytes\n", (int)len);
    this->console_write(this->UART2_TX_Buffer);
 
}

uint32_t Utils::usb_audio_write_tx_data(char* buf, uint32_t len)
{
   
     return len;
}

void Utils::ADC_to_MIC(void)
{
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_buffer, OSA_BUF_SIZE * (AUDIO_IN_PACKET / 2)); // Start ADC transfer into oversampling buffer
}

uint8_t Utils::USBD_AUDIO_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
  USBD_AUDIO_HandleTypeDef *haudio;
  uint8_t retval = USBD_OK;
	
  haudio = (USBD_AUDIO_HandleTypeDef*) pdev->pClassData;
	
	if (epnum == (AUDIO_IN_EP2 & 0x7F))
  {
		//haudio->buffer = !haudio->buffer;    // also serves as init to 1 or 0
		uint16_t prev = (AUDIO_IN_PACKET / 2) * !haudio->buffer;  // invert in_buffer_half again to get the previous value 
		                                                                  // the double inversion serves to get rid of possible malloc
																																			// artifacts in in_buffer_half, not initialized at the start

		ADC_to_MIC();
		
		USBD_LL_FlushEP  (pdev, AUDIO_IN_EP2);
		USBD_LL_Transmit (pdev, AUDIO_IN_EP2, (uint8_t*)(haudio->buffer + prev), AUDIO_IN_PACKET);

    return retval;
	}

  return retval;
}

