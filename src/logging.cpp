#include "logging.h"
 
UART_HandleTypeDef Logging::huart2;
uint8_t Logging::UART2_RX_Buffer[26];
char Logging::UART2_TX_Buffer[128];

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  
    sprintf(Logging::UART2_TX_Buffer, "Roger, got 26 chars, got for next 26!\n");
    HAL_UART_Transmit(&Logging::huart2, (uint8_t *)Logging::UART2_TX_Buffer, strlen(Logging::UART2_TX_Buffer), 10);
    HAL_UART_Transmit(&Logging::huart2, Logging::UART2_RX_Buffer, 26, 10);
    HAL_UART_Receive_DMA(&Logging::huart2, Logging::UART2_RX_Buffer, 26);
}

Logging* Logging::getInstance()  {
    if (instancePtr == NULL) {
         instancePtr = new Logging(); 
           return instancePtr; 
    }
    else{
       return instancePtr;
    }
}

Logging::Logging(){
  this->init_uart();
}

void Logging::console_write(const char *msg){
    HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), 10);
}

void Logging::error(const char *msg){
    HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), 10);
}

void Logging::warning(const char *msg){
    HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), 10);
}

void Logging::info(const char *msg){
    HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), 10);
}

void Logging::debug(const char *msg){
    HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), 10);
}

void Logging::init_uart(){
  
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

void Logging::error_handler(void){
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}