#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdio.h>
#include <string.h>
#include <usbd_def.h> 



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

 class Logging {

  public:
    static Logging* getInstance();
    void console_write(const char *msg);
    Logging();
    Logging(const Logging& obj) = delete;
     
    static UART_HandleTypeDef huart2;
     
     int log_level;
     
     void error(const char * msg);
     void warning(const char * msg);
     void info(const char * msg);
     void debug(const char * msg);

     void init_uart();
     void error_handler(void);
     static uint8_t UART2_RX_Buffer[26];
     static char UART2_TX_Buffer[128];
     DMA_HandleTypeDef hdma_usart2_rx;

  private:
    static Logging* instancePtr; 
};

#endif