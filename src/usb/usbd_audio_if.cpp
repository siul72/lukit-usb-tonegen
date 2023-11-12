 
#include "usb/usbd_audio_if.h"
#include "logging.h"

extern USBD_HandleTypeDef hUsbDeviceFS;
 

static int8_t AUDIO_Init(uint32_t AudioFreq, uint32_t Volume, uint32_t options);
static int8_t AUDIO_DeInit(uint32_t options);
static int8_t AUDIO_AudioCmd(uint8_t* pbuf, uint32_t size, uint8_t cmd);
static int8_t AUDIO_VolumeCtl(uint8_t vol);
static int8_t AUDIO_MuteCtl(uint8_t cmd);
static int8_t AUDIO_PeriodicTC(uint8_t *pbuf, uint32_t size, uint8_t cmd);
static int8_t AUDIO_GetState(void);
  
static int8_t AUDIO_Record();
static int8_t AUDIO_Stop();
static int8_t AUDIO_Pause();
static int8_t AUDIO_Resume();
static int8_t AUDIO_CommandMgr(uint8_t cmd);
 

/* USBD_AUDIO_ItfTypeDef USBD_AUDIO_fops =
{
  AUDIO_Init,
  AUDIO_DeInit,
  AUDIO_AudioCmd,
  AUDIO_VolumeCtl,
  AUDIO_MuteCtl,
  AUDIO_PeriodicTC,
  AUDIO_GetState,
}; */

 

USBD_AUDIO_ItfTypeDef USBD_AUDIO_fops = { AUDIO_Init, AUDIO_DeInit, AUDIO_AudioCmd, 
AUDIO_VolumeCtl, AUDIO_MuteCtl,  AUDIO_PeriodicTC, AUDIO_GetState, AUDIO_Record,
    AUDIO_Stop, AUDIO_Pause, AUDIO_Resume, AUDIO_CommandMgr, }; 

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initializes the AUDIO media low layer over USB FS IP
  * @param  AudioFreq: Audio frequency used to play the audio stream.
  * @param  Volume: Initial volume level (from 0 (Mute) to 100 (Max))
  * @param  options: Reserved for future use
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t AUDIO_Init(uint32_t AudioFreq, uint32_t Volume, uint32_t options)
{
  /* USER CODE BEGIN 0 */
  UNUSED(AudioFreq);
  UNUSED(Volume);
  UNUSED(options);
  return (USBD_OK);
  /* USER CODE END 0 */
}

/**
  * @brief  De-Initializes the AUDIO media low layer
  * @param  options: Reserved for future use
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t AUDIO_DeInit(uint32_t options)
{
  /* USER CODE BEGIN 1 */
  UNUSED(options);
  return (USBD_OK);
  /* USER CODE END 1 */
}

/**
  * @brief  Handles AUDIO command.
  * @param  pbuf: Pointer to buffer of data to be sent
  * @param  size: Number of data to be sent (in bytes)
  * @param  cmd: Command opcode
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t AUDIO_AudioCmd(uint8_t* pbuf, uint32_t size, uint8_t cmd)
{
  /* USER CODE BEGIN 2 */
  switch(cmd)
  {
    case AUDIO_CMD_START:
    break;

    case AUDIO_CMD_PLAY:
    break;
  }
  UNUSED(pbuf);
  UNUSED(size);
  UNUSED(cmd);
  return (USBD_OK);
  /* USER CODE END 2 */
}

/**
  * @brief  Controls AUDIO Volume.
  * @param  vol: volume level (0..100)
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t AUDIO_VolumeCtl(uint8_t vol)
{
  /* USER CODE BEGIN 3 */
  UNUSED(vol);
  return (USBD_OK);
  /* USER CODE END 3 */
}

/**
  * @brief  Controls AUDIO Mute.
  * @param  cmd: command opcode
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t AUDIO_MuteCtl(uint8_t cmd)
{
  /* USER CODE BEGIN 4 */
  UNUSED(cmd);
  return (USBD_OK);
  /* USER CODE END 4 */
}

/**
  * @brief  AUDIO_PeriodicT_FS
  * @param  cmd: Command opcode
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t AUDIO_PeriodicTC(uint8_t *pbuf, uint32_t size, uint8_t cmd)
{
   
  UNUSED(pbuf);
  UNUSED(size);
  UNUSED(cmd);

  Logging::getInstance()->console_write("AUDIO_PeriodicTC_FS");
  
  return (USBD_OK);
 
}

/**
  * @brief  Gets AUDIO State.
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t AUDIO_GetState(void)
{
  /* USER CODE BEGIN 6 */
  return (USBD_OK);
  /* USER CODE END 6 */
}

static int8_t AUDIO_Record()
{
  return (USBD_OK);
}

static int8_t AUDIO_Stop()
{
  return (USBD_OK);
}

static int8_t AUDIO_Pause()
{
  return (USBD_OK);
}

int8_t AUDIO_Resume()
{
  return (USBD_OK);
}

int8_t AUDIO_CommandMgr(uint8_t cmd)
{
  UNUSED(cmd);
  return (USBD_OK);
}

void TransferComplete_CallBack_FS(void)
{
  Logging::getInstance()->console_write("TransferComplete_CallBack_FS");
  USBD_AUDIO_Sync(&hUsbDeviceFS, AUDIO_OFFSET_FULL);
  /* USER CODE END 7 */
}
 
void HalfTransfer_CallBack_FS(void)
{
  
  Logging::getInstance()->console_write("HalfTransfer_CallBack_FS");
  USBD_AUDIO_Sync(&hUsbDeviceFS, AUDIO_OFFSET_HALF);
  /* USER CODE END 8 */
}
 