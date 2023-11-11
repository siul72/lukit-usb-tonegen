#include "usbd_audio.h"
#include "usbd_ctlreq.h"
#include "utils.h"
 
extern unsigned char *USBDevice;
extern size_t USBDeviceSize;
extern unsigned char *USBConfiguration;
extern size_t USBConfigurationSize;
 

volatile uint32_t tx_flag = 1;
 
#define AUDIO_SAMPLE_FREQ(frq)         (uint8_t)(frq), (uint8_t)((frq >> 8)), (uint8_t)((frq >> 16))

#define AUDIO_PACKET_SZE(frq)          (uint8_t)(((frq * 2U * 2U)/1000U) & 0xFFU), \
                                       (uint8_t)((((frq * 2U * 2U)/1000U) >> 8) & 0xFFU)

static uint8_t IsocInBuffDummy[48 * 4 * 2];
static int16_t MUTE_CUR;
static int16_t VOL_CUR;
static uint8_t EQ_CUR[36];


/** @defgroup USBD_AUDIO_Private_FunctionPrototypes
  * @{
  */
static uint8_t USBD_AUDIO_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static USBD_StatusTypeDef USBD_AUDIO_Init_IN(USBD_HandleTypeDef *pdev, USBD_AUDIO_HandleTypeDef *haudio);
static USBD_StatusTypeDef USBD_AUDIO_Init_OUT(USBD_HandleTypeDef *pdev, USBD_AUDIO_HandleTypeDef *haudio);
static uint8_t USBD_AUDIO_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx);

static uint8_t USBD_AUDIO_Setup(USBD_HandleTypeDef *pdev,
                                USBD_SetupReqTypedef *req);

static uint8_t *USBD_AUDIO_GetCfgDesc(uint16_t *length);
static uint8_t *USBD_AUDIO_GetDeviceQualifierDesc(uint16_t *length);
static uint8_t USBD_AUDIO_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t USBD_AUDIO_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t USBD_AUDIO_EP0_RxReady(USBD_HandleTypeDef *pdev);
static uint8_t USBD_AUDIO_EP0_TxReady(USBD_HandleTypeDef *pdev);
static uint8_t USBD_AUDIO_SOF(USBD_HandleTypeDef *pdev);

static uint8_t USBD_AUDIO_IsoINIncomplete(USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t USBD_AUDIO_IsoOutIncomplete(USBD_HandleTypeDef *pdev, uint8_t epnum);
static void AUDIO_REQ_GetCurrent(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
static void AUDIO_REQ_SetCurrent(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
static void AUDIO_REQ_GetMaximum(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
static void AUDIO_REQ_GetMinimum(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
static void AUDIO_REQ_GetResolution(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
 
USBD_ClassTypeDef USBD_AUDIO =
{
  USBD_AUDIO_Init,
  USBD_AUDIO_DeInit,
  USBD_AUDIO_Setup,
  USBD_AUDIO_EP0_TxReady,
  USBD_AUDIO_EP0_RxReady,
  USBD_AUDIO_DataIn,
  USBD_AUDIO_DataOut,
  USBD_AUDIO_SOF,
  USBD_AUDIO_IsoINIncomplete,
  USBD_AUDIO_IsoOutIncomplete,
  USBD_AUDIO_GetCfgDesc,
  USBD_AUDIO_GetCfgDesc,
  USBD_AUDIO_GetCfgDesc,
  USBD_AUDIO_GetDeviceQualifierDesc,
};
  
/* USB Standard Device Descriptor */
__ALIGN_BEGIN static uint8_t USBD_AUDIO_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] __ALIGN_END =
{
  USB_LEN_DEV_QUALIFIER_DESC,
  USB_DESC_TYPE_DEVICE_QUALIFIER,
  0x00,
  0x02,
  0x00,
  0x00,
  0x00,
  0x40,
  0x01,
  0x00,
};

static USBD_StatusTypeDef USBD_AUDIO_Init_IN(USBD_HandleTypeDef *pdev, USBD_AUDIO_HandleTypeDef *haudio){

  uint16_t packet_dim = haudio->paketDimension;
  uint16_t wr_rd_offset = (AUDIO_IN_PACKET_NUM / 2) * haudio->dataAmount / haudio->paketDimension;
  haudio->wr_ptr = wr_rd_offset * packet_dim;
  haudio->rd_ptr = 0;
  haudio->timeout = 0;
  haudio->alt_setting = 0U;
  haudio->offset = AUDIO_OFFSET_UNKNOWN;
  haudio->rd_ptr = 0U;
  haudio->rd_enable = 0U;

   USBD_StatusTypeDef ret_status = USBD_OK;

  ret_status = USBD_LL_OpenEP(pdev, AUDIO_MIC_IN_USB_EP,  USBD_EP_TYPE_ISOC, AUDIO_IN_PACKET);
  if(ret_status != USBD_OK){

    return ret_status;
  }
  ret_status = USBD_LL_FlushEP(pdev, AUDIO_MIC_IN_USB_EP);
  if(ret_status != USBD_OK){

    return ret_status;
  }
  ret_status = USBD_LL_Transmit(pdev, AUDIO_MIC_IN_USB_EP, IsocInBuffDummy, packet_dim);
  if(ret_status != USBD_OK){

    return ret_status;
  }

  pdev->ep_in[AUDIO_MIC_IN_USB_EP & 0xFU].is_used = 1U;
  haudio->state = STATE_USB_IDLE;

    /** 
   * Set tx_flag 1 to block feedback transmission in SOF handler since 
   * device is not ready.
   */
  tx_flag = 1U;


  return ret_status;

}

static USBD_StatusTypeDef USBD_AUDIO_Init_OUT(USBD_HandleTypeDef *pdev, USBD_AUDIO_HandleTypeDef *haudio){

    if (pdev->dev_speed == USBD_SPEED_HIGH)
    {
      pdev->ep_out[AUDIO_OUT_EP1 & 0xFU].bInterval = AUDIO_HS_BINTERVAL;
    }
    else   /* LOW and FULL-speed endpoints */
    {
      pdev->ep_out[AUDIO_OUT_EP1 & 0xFU].bInterval = AUDIO_FS_BINTERVAL;
    }

    USBD_StatusTypeDef ret_status = USBD_OK;
    ret_status = USBD_LL_OpenEP(pdev, AUDIO_OUT_EP1, USBD_EP_TYPE_ISOC, AUDIO_OUT_PACKET);
    if(ret_status != USBD_OK){

    return ret_status;
  }
    pdev->ep_out[AUDIO_OUT_EP1 & 0xFU].is_used = 1U;

    /* Prepare Out endpoint to receive 1st packet */
     ret_status = USBD_LL_PrepareReceive(pdev, AUDIO_OUT_EP1, haudio->buffer,  AUDIO_OUT_PACKET);
     if(ret_status != USBD_OK){

    return ret_status;
    }

    return ret_status;
}


static uint8_t USBD_AUDIO_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
  UNUSED(cfgidx);
  USBD_AUDIO_HandleTypeDef *haudio;
  USBD_StatusTypeDef ret_status = USBD_OK;

  sprintf(Utils::UART2_TX_Buffer, ">>USBD_AUDIO_Init\n");
  Utils::getInstance()->console_write(Utils::UART2_TX_Buffer);

  /* Allocate Audio structure */
  haudio = (USBD_AUDIO_HandleTypeDef *)USBD_malloc(sizeof(USBD_AUDIO_HandleTypeDef));

  if (haudio == NULL)
  {
    pdev->pClassData = NULL;
    return USBD_EMEM;
  }

  pdev->pClassData = (void *)haudio;
  if (haudio->state != STATE_USB_WAITING_FOR_INIT) {
    return USBD_FAIL;
  }

  if (((USBD_AUDIO_ItfTypeDef *)pdev->pUserData)->Init(haudio->frequency,
                                                       AUDIO_DEFAULT_VOLUME,
                                                       haudio->channels) != 0U)
  {
    return USBD_FAIL;
  }

  
  ret_status = USBD_AUDIO_Init_IN(pdev,  haudio);
  if(ret_status != USBD_OK){

    return ret_status;
  }
  ret_status = USBD_AUDIO_Init_OUT(pdev, haudio);
  if(ret_status != USBD_OK){

    return ret_status;
  }

  sprintf(Utils::UART2_TX_Buffer, "<<USBD_AUDIO_Init OK\n");
  Utils::getInstance()->console_write(Utils::UART2_TX_Buffer);
    
  return USBD_OK;
}

/**
  * @brief  USBD_AUDIO_Init
  *         DeInitialize the AUDIO layer
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t USBD_AUDIO_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
  UNUSED(cfgidx);

  /* Open EP OUT */
  (void)USBD_LL_CloseEP(pdev, AUDIO_OUT_EP1);
  pdev->ep_out[AUDIO_OUT_EP1 & 0xFU].is_used = 0U;
  pdev->ep_out[AUDIO_OUT_EP1 & 0xFU].bInterval = 0U;

  /* DeInit  physical Interface components */
   USBD_LL_CloseEP(pdev, AUDIO_IN_EP2);
   pdev->ep_out[AUDIO_OUT_EP1 & 0xFU].is_used = 0U;
   pdev->ep_out[AUDIO_OUT_EP1 & 0xFU].bInterval = 0U;
    /* DeInit  physical Interface components */
    if (pdev->pClassData != NULL) {
      USBD_AUDIO_HandleTypeDef * haudio = (USBD_AUDIO_HandleTypeDef *)pdev->pClassData;
      ((USBD_AUDIO_ItfTypeDef *)pdev->pUserData)->DeInit(0);
      haudio->state = STATE_USB_WAITING_FOR_INIT;
      (void)USBD_free(pdev->pClassData);
      pdev->pClassData = NULL;
    }
 
  return (uint8_t)USBD_OK;
}

/**
  * @brief  USBD_AUDIO_Setup
  *         Handle the AUDIO specific requests
  * @param  pdev: instance
  * @param  req: usb requests
  * @retval status
  */
static uint8_t USBD_AUDIO_Setup(USBD_HandleTypeDef *pdev,
                                USBD_SetupReqTypedef *req)
{

  USBD_AUDIO_HandleTypeDef *haudio;
  uint16_t len;
  uint8_t *pbuf;
  uint8_t ret = USBD_OK;
  haudio = (USBD_AUDIO_HandleTypeDef *)pdev->pClassData;

  sprintf(Utils::UART2_TX_Buffer, ">>USBD_AUDIO_Setup bmRequest=0x%02x bRequest=0x%02x\n", (u_int8_t)(req->bmRequest & USB_REQ_TYPE_MASK), req->bRequest);
  Utils::getInstance()->console_write(Utils::UART2_TX_Buffer);

  switch (req->bmRequest & USB_REQ_TYPE_MASK) {
  /* AUDIO Class Requests -------------------------------*/
  case USB_REQ_TYPE_CLASS:
    switch (req->bRequest) {
    case AUDIO_REQ_GET_CUR:
      AUDIO_REQ_GetCurrent(pdev, req);
      break;

    case AUDIO_REQ_SET_CUR:
      AUDIO_REQ_SetCurrent(pdev, req);
      break;

    case AUDIO_REQ_GET_MIN:
      AUDIO_REQ_GetMinimum(pdev, req);
      break;

    case AUDIO_REQ_GET_MAX:
      AUDIO_REQ_GetMaximum(pdev, req);
      break;

    case AUDIO_REQ_GET_RES:
      AUDIO_REQ_GetResolution(pdev, req);
      break;

    default:
      USBD_CtlError(pdev, req);
      return USBD_FAIL;
    }
    break;

    /* Standard Requests -------------------------------*/
  case USB_REQ_TYPE_STANDARD:
    switch (req->bRequest) {
      case USB_REQ_GET_DESCRIPTOR:
        if ((req->wValue >> 8) == AUDIO_DESCRIPTOR_TYPE) {

          //pbuf = USBD_AUDIO_CfgDesc + 18;
          pbuf = USBConfiguration + 18;
          len = MIN(USBConfigurationSize, req->wLength);

          USBD_CtlSendData(pdev, pbuf, len);
        }
      break;

      case USB_REQ_GET_INTERFACE:
        sprintf(Utils::UART2_TX_Buffer, "USB_REQ_GET_INTERFACE setting=%d\n", (int)haudio->alt_setting);
        Utils::getInstance()->console_write(Utils::UART2_TX_Buffer);
        USBD_CtlSendData(pdev, (uint8_t*) haudio->alt_setting, 1);
      break;

      case USB_REQ_SET_INTERFACE:
        if ((uint8_t) (req->wValue) < USBD_MAX_NUM_INTERFACES) {
          haudio->alt_setting = (uint8_t) (req->wValue);
          if (haudio->alt_setting == 0){
            tx_flag = 1U;
          }

          if (haudio->alt_setting == 1){
            tx_flag = 0U;
            sprintf(Utils::UART2_TX_Buffer, "START MIC TX\n");
            Utils::getInstance()->console_write(Utils::UART2_TX_Buffer);
          } 
          sprintf(Utils::UART2_TX_Buffer, "haudio->alt_setting=%d tx_flag=%d\n", (int)haudio->alt_setting, (int)tx_flag);
          Utils::getInstance()->console_write(Utils::UART2_TX_Buffer);
        } else {
          /* Call the error management function (command will be nacked */
          USBD_CtlError(pdev, req);
        }
      break;
    }
  }

  sprintf(Utils::UART2_TX_Buffer, "<<USBD_AUDIO_Setup return=%d\n", ret);
  Utils::getInstance()->console_write(Utils::UART2_TX_Buffer);
  return ret;
}

 
static uint8_t *USBD_AUDIO_GetCfgDesc(uint16_t *length)
{
   *length = (uint16_t)USBConfigurationSize;
  return USBConfiguration;
}

/**
  * @brief  USBD_AUDIO_DataIn
  *         handle data IN Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t USBD_AUDIO_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
  USBD_AUDIO_HandleTypeDef *haudio;
  haudio = (USBD_AUDIO_HandleTypeDef *)pdev->pClassData;
  uint32_t length_usb_pck;
  //uint16_t app;
  //uint16_t IsocInWr_app = haudio->wr_ptr;
  //uint16_t true_dim = haudio->buffer_length;
  uint16_t packet_dim = haudio->paketDimension;
  //uint16_t channels = haudio->channels;

  //sprintf(Utils::UART2_TX_Buffer, ">>USBD_AUDIO_DataIn ts=%d\n", (int)HAL_GetTick());
  //Utils::getInstance()->console_write(Utils::UART2_TX_Buffer);
   
  length_usb_pck = packet_dim;
  haudio->timeout = 0;
  if (epnum == (AUDIO_MIC_IN_USB_EP & 0x7F)) {
      //Utils::getInstance()->get_sine_sample();

    if (haudio->state == STATE_USB_IDLE) {
      haudio->state = STATE_USB_REQUESTS_STARTED;
      //((USBD_AUDIO_ItfTypeDef*) pdev->pUserData)->Record();
      haudio->state = STATE_USB_BUFFER_WRITE_STARTED;
    }
    if (haudio->state == STATE_USB_BUFFER_WRITE_STARTED) {
      Utils::getInstance()->get_sine_sample();
     /*    haudio->rd_ptr = haudio->rd_ptr % (true_dim);
        if (IsocInWr_app < haudio->rd_ptr) {
          app = ((true_dim) - haudio->rd_ptr) + IsocInWr_app;
        } else {
          app = IsocInWr_app - haudio->rd_ptr;
        }
        if (app >= (packet_dim * haudio->upper_treshold)) {
          length_usb_pck += channels * 2;
        } else if (app <= (packet_dim * haudio->lower_treshold)) {
          length_usb_pck -= channels * 2;
        }
        USBD_LL_Transmit(pdev, AUDIO_MIC_IN_USB_EP, (uint8_t*) (&haudio->buffer[haudio->rd_ptr]), length_usb_pck);
        haudio->rd_ptr += length_usb_pck;

        if (app < haudio->buffer_length / 10) {
          ((USBD_AUDIO_ItfTypeDef*) pdev->pUserData)->Stop();
          haudio->state = STATE_USB_IDLE;
          haudio->timeout = 0;
          memset(haudio->buffer, 0, (haudio->buffer_length + haudio->dataAmount)); 
        } */
    } else {
      USBD_LL_Transmit(pdev, AUDIO_MIC_IN_USB_EP, IsocInBuffDummy, length_usb_pck);
    }
  }
  
  return (uint8_t)USBD_OK;
}

/**
  * @brief  USBD_AUDIO_EP0_RxReady
  *         handle EP0 Rx Ready event
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t USBD_AUDIO_EP0_RxReady(USBD_HandleTypeDef *pdev)
{
  USBD_AUDIO_HandleTypeDef *haudio;
  haudio = (USBD_AUDIO_HandleTypeDef *)pdev->pClassData;

  sprintf(Utils::UART2_TX_Buffer, ">>USBD_AUDIO_EP0_RxReady\n");
  Utils::getInstance()->console_write(Utils::UART2_TX_Buffer);

  if (haudio == NULL)
  {
    return (uint8_t)USBD_FAIL;
  }
    //SET_CUR, SET_MIN, SET_MAX, SET_RES, SET_MEM
  if (haudio->control.cmd == AUDIO_REQ_SET_CUR)
  {
    
    if (haudio->control.unit == AUDIO_OUT_STREAMING_CTRL)
    {
      ((USBD_AUDIO_ItfTypeDef *)pdev->pUserData)->MuteCtl(haudio->control.data[0]);
      haudio->control.cmd = 0U;
      haudio->control.len = 0U;
    }

    if (haudio->control.unit == AUDIO_TERMINAL_FEATURE_MIC_IN)
    {
      ((USBD_AUDIO_ItfTypeDef *)pdev->pUserData)->MuteCtl(haudio->control.data[0]);
      haudio->control.cmd = 0U;
      haudio->control.len = 0U;
    }
 
  }

  return (uint8_t)USBD_OK;
}
/**
  * @brief  USBD_AUDIO_EP0_TxReady
  *         handle EP0 TRx Ready event
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t USBD_AUDIO_EP0_TxReady(USBD_HandleTypeDef *pdev)
{
  UNUSED(pdev);
  //sprintf(Utils::UART2_TX_Buffer, ">>USBD_AUDIO_EP0_TxReady\n");
  //Utils::getInstance()->console_write(Utils::UART2_TX_Buffer);

  /* Only OUT control data are processed */
  return (uint8_t)USBD_OK;
}
/**
  * @brief  USBD_AUDIO_SOF
  *         handle SOF event
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t USBD_AUDIO_SOF(USBD_HandleTypeDef *pdev)
{
  UNUSED(pdev); 
  return (uint8_t)USBD_OK;
}

/**
  * @brief  USBD_AUDIO_SOF
  *         handle SOF event
  * @param  pdev: device instance
  * @retval status
  */
void USBD_AUDIO_Sync(USBD_HandleTypeDef *pdev, AUDIO_OffsetTypeDef offset)
{
  USBD_AUDIO_HandleTypeDef *haudio;
  uint32_t BufferSize = AUDIO_TOTAL_BUF_SIZE / 2U;

  sprintf(Utils::UART2_TX_Buffer, ">>USBD_AUDIO_Sync\n");
  Utils::getInstance()->console_write(Utils::UART2_TX_Buffer);


  if (pdev->pClassData == NULL)
  {
    return;
  }

  haudio = (USBD_AUDIO_HandleTypeDef *)pdev->pClassData;

  haudio->offset = offset;

  if (haudio->rd_enable == 1U)
  {
    haudio->rd_ptr += (uint16_t)BufferSize;

    if (haudio->rd_ptr == AUDIO_TOTAL_BUF_SIZE)
    {
      /* roll back */
      haudio->rd_ptr = 0U;
    }
  }

  if (haudio->rd_ptr > haudio->wr_ptr)
  {
    if ((haudio->rd_ptr - haudio->wr_ptr) < AUDIO_OUT_PACKET)
    {
      BufferSize += 4U;
    }
    else
    {
      if ((haudio->rd_ptr - haudio->wr_ptr) > (AUDIO_TOTAL_BUF_SIZE - AUDIO_OUT_PACKET))
      {
        BufferSize -= 4U;
      }
    }
  }
  else
  {
    if ((haudio->wr_ptr - haudio->rd_ptr) < AUDIO_OUT_PACKET)
    {
      BufferSize -= 4U;
    }
    else
    {
      if ((haudio->wr_ptr - haudio->rd_ptr) > (AUDIO_TOTAL_BUF_SIZE - AUDIO_OUT_PACKET))
      {
        BufferSize += 4U;
      }
    }
  }

  if (haudio->offset == AUDIO_OFFSET_FULL)
  {
    ((USBD_AUDIO_ItfTypeDef *)pdev->pUserData)->AudioCmd(&haudio->buffer[0],
                                                         BufferSize, AUDIO_CMD_PLAY);
    haudio->offset = AUDIO_OFFSET_NONE;
  }
}

/**
  * @brief  USBD_AUDIO_IsoINIncomplete
  *         handle data ISO IN Incomplete event
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t USBD_AUDIO_IsoINIncomplete(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
  UNUSED(pdev);
  UNUSED(epnum);

  return (uint8_t)USBD_OK;
}
/**
  * @brief  USBD_AUDIO_IsoOutIncomplete
  *         handle data ISO OUT Incomplete event
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t USBD_AUDIO_IsoOutIncomplete(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
  UNUSED(pdev);
  UNUSED(epnum);
  
  //sprintf(Utils::UART2_TX_Buffer, ">>USBD_AUDIO_IsoOutIncomplete epnum=0x%02x\n",epnum);
  //Utils::getInstance()->console_write(Utils::UART2_TX_Buffer);

  return (uint8_t)USBD_OK;
}
/**
  * @brief  USBD_AUDIO_DataOut
  *         handle data OUT Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t USBD_AUDIO_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
  uint16_t PacketSize;
  USBD_AUDIO_HandleTypeDef *haudio;

  haudio = (USBD_AUDIO_HandleTypeDef *)pdev->pClassData;

  sprintf(Utils::UART2_TX_Buffer, ">>USBD_AUDIO_DataOut epnum=0x%02x\n",epnum);
  Utils::getInstance()->console_write(Utils::UART2_TX_Buffer);

  if (haudio == NULL)
  {
    return (uint8_t)USBD_FAIL;
  }

  if (epnum == AUDIO_OUT_EP1)
  {
    /* Get received data packet length */
    PacketSize = (uint16_t)USBD_LL_GetRxDataSize(pdev, epnum);

    /* Packet received Callback */
    ((USBD_AUDIO_ItfTypeDef *)pdev->pUserData)->PeriodicTC(&haudio->buffer[haudio->wr_ptr],
                                                           PacketSize, AUDIO_OUT_TC);

    /* Increment the Buffer pointer or roll it back when all buffers are full */
    haudio->wr_ptr += PacketSize;

    if (haudio->wr_ptr == AUDIO_TOTAL_BUF_SIZE)
    {
      /* All buffers are full: roll back */
      haudio->wr_ptr = 0U;

      if (haudio->offset == AUDIO_OFFSET_UNKNOWN)
      {
        ((USBD_AUDIO_ItfTypeDef *)pdev->pUserData)->AudioCmd(&haudio->buffer[0],
                                                             AUDIO_TOTAL_BUF_SIZE / 2U,
                                                             AUDIO_CMD_START);
        haudio->offset = AUDIO_OFFSET_NONE;
      }
    }

    if (haudio->rd_enable == 0U)
    {
      if (haudio->wr_ptr == (AUDIO_TOTAL_BUF_SIZE / 2U))
      {
        haudio->rd_enable = 1U;
      }
    }

    /* Prepare Out endpoint to receive next audio packet */
    (void)USBD_LL_PrepareReceive(pdev, AUDIO_OUT_EP1,
                                 &haudio->buffer[haudio->wr_ptr],
                                 AUDIO_OUT_PACKET);
  }

  return (uint8_t)USBD_OK;
}

/**
  * @brief  AUDIO_Req_GetCurrent
  *         Handles the GET_CUR Audio control request.
  * @param  pdev: instance
  * @param  req: setup class request
  * @retval status
  */
static void AUDIO_REQ_GetCurrent(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{
  
  USBD_AUDIO_HandleTypeDef *haudio = (USBD_AUDIO_HandleTypeDef *)pdev->pClassData;
  uint8_t bControlSelector = req->wValue >> 8;

  sprintf(Utils::UART2_TX_Buffer, ">>AUDIO_REQ_GetCurrent bControlSelector=0x%02x\n", bControlSelector);
  Utils::getInstance()->console_write(Utils::UART2_TX_Buffer);

  char buffer[20];
  itoa(bControlSelector, buffer, 10);
  puts(buffer);

  switch (bControlSelector) {

  case FEATURE_VOLUME:
  //to do: Get the volume per device
    (haudio->control.data)[0] = (uint16_t) AUDIO_DEFAULT_VOLUME & 0xFF;
    (haudio->control.data)[1] = ((uint16_t) AUDIO_DEFAULT_VOLUME & 0xFF00) >> 8;
    break;

  case FEATURE_GRAPHIC_EQ:
    
    break;

  }

  USBD_CtlSendData(pdev, haudio->control.data, req->wLength);
  
}

/**
  * @brief  AUDIO_Req_SetCurrent
  *         Handles the SET_CUR Audio control request.
  * @param  pdev: instance
  * @param  req: setup class request
  * @retval status
  */
static void AUDIO_REQ_SetCurrent(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{
  USBD_AUDIO_HandleTypeDef *haudio;
  haudio = (USBD_AUDIO_HandleTypeDef *)pdev->pClassData;
  uint8_t bControlSelector = req->wValue >> 8;

  sprintf(Utils::UART2_TX_Buffer, ">>AUDIO_REQ_SetCurrent bControlSelector=0x%02x\n", bControlSelector);
  Utils::getInstance()->console_write(Utils::UART2_TX_Buffer);

  if (haudio == NULL)
  {
    return;
  }

  switch (bControlSelector) {

  case FEATURE_MUTE:
    haudio->control.cmd = AUDIO_CTRL_REQ_SET_CUR_MUTE; /* Set the request value */
    haudio->control.len = req->wLength; /* Set the request data length */
    haudio->control.unit = HIBYTE(req->wIndex); /* Set the request target unit */
    USBD_CtlPrepareRx(pdev, (uint8_t*)&MUTE_CUR, req->wLength);
    sprintf(Utils::UART2_TX_Buffer, "mute=0x%04x\n", MUTE_CUR);
    Utils::getInstance()->console_write(Utils::UART2_TX_Buffer);
    break;

  case FEATURE_VOLUME:
    haudio->control.cmd = AUDIO_CTRL_REQ_SET_CUR_VOLUME; /* Set the request value */
    haudio->control.len = req->wLength; /* Set the request data length */
    haudio->control.unit = HIBYTE(req->wIndex); /* Set the request target unit */
    USBD_CtlPrepareRx(pdev, (uint8_t*) &VOL_CUR, req->wLength);
    sprintf(Utils::UART2_TX_Buffer, "mute=0x%04x\n", MUTE_CUR);
    break;

  case FEATURE_GRAPHIC_EQ:
    haudio->control.cmd = AUDIO_CTRL_REQ_SET_CUR_EQUALIZER; /* Set the request value */
    haudio->control.len = req->wLength; /* Set the request data length */
    haudio->control.unit = HIBYTE(req->wIndex); /* Set the request target unit */
    USBD_CtlPrepareRx(pdev, (uint8_t*) EQ_CUR, req->wLength);
    break;
  }
}

/**
 * @brief  AUDIO_REQ_GetMaximum
 *         Handles the VOL_MAX Audio control request.
 * @param  pdev: instance
 * @param  req: setup class request
 * @retval status
 */
static void AUDIO_REQ_GetMaximum(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req) {

  USBD_AUDIO_HandleTypeDef *haudio = (USBD_AUDIO_HandleTypeDef *)pdev->pClassData;
  uint8_t bControlSelector = req->wValue >> 8;

  sprintf(Utils::UART2_TX_Buffer, ">>AUDIO_REQ_GetMaximum bControlSelector=0x%02x\n", bControlSelector);
  Utils::getInstance()->console_write(Utils::UART2_TX_Buffer);

  switch (bControlSelector) {

  case FEATURE_VOLUME:
    (haudio->control.data)[0] = (uint16_t) VOL_MAX & 0xFF;
    (haudio->control.data)[1] = ((uint16_t) VOL_MAX & 0xFF00) >> 8;
    break;

  case FEATURE_GRAPHIC_EQ:
    (haudio->control.data)[0] = 0xff;   // the first 10 bands are supported
    (haudio->control.data)[1] = 0x3;
    (haudio->control.data)[2] = 0;
    (haudio->control.data)[3] = 0;

    for (uint8_t i = 0; i < 10; i++) {
      (haudio->control.data)[i + 4] = 48;    // max = 12dB, units here are 0.25dB
    }
    break;
  }

  USBD_CtlSendData(pdev, haudio->control.data, req->wLength);
}

/**
 * @brief  AUDIO_REQ_GetMinimum
 *         Handles the VOL_MIN Audio control request.
 * @param  pdev: instance
 * @param  req: setup class request
 * @retval status
 */
static void AUDIO_REQ_GetMinimum(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req) {

  USBD_AUDIO_HandleTypeDef *haudio = (USBD_AUDIO_HandleTypeDef *)pdev->pClassData;
  uint8_t bControlSelector = req->wValue >> 8;

  sprintf(Utils::UART2_TX_Buffer, ">>AUDIO_REQ_GetMinimum bControlSelector=0x%02x\n", bControlSelector);
  Utils::getInstance()->console_write(Utils::UART2_TX_Buffer);

  switch (bControlSelector) {

  case FEATURE_VOLUME:
    (haudio->control.data)[0] = (uint16_t) VOL_MIN & 0xFF;
    (haudio->control.data)[1] = ((uint16_t) VOL_MIN & 0xFF00) >> 8;
    /* Send the current mute state */
    break;

  case FEATURE_GRAPHIC_EQ:
    (haudio->control.data)[0] = 0xff;   // the first 10 bands are supported
    (haudio->control.data)[1] = 0x3;
    (haudio->control.data)[2] = 0;
    (haudio->control.data)[3] = 0;

    for (uint8_t i = 0; i < 10; i++) {
      (haudio->control.data)[i + 4] = 0xD0;    // min = -12dB, units here are 0.25dB (0xD0 == -48)
    }
    break;
  }

  USBD_CtlSendData(pdev, haudio->control.data, req->wLength);
}

/**
 * @brief  AUDIO_Req_GetResolution
 *         Handles the VOL_RES Audio control request.
 * @param  pdev: instance
 * @param  req: setup class request
 * @retval status
 */
static void AUDIO_REQ_GetResolution(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req) {

  USBD_AUDIO_HandleTypeDef *haudio = (USBD_AUDIO_HandleTypeDef *)pdev->pClassData;
  uint8_t bControlSelector = req->wValue >> 8;

  sprintf(Utils::UART2_TX_Buffer, ">>AUDIO_REQ_GetResolution bControlSelector=0x%02x\n", bControlSelector);
  Utils::getInstance()->console_write(Utils::UART2_TX_Buffer);

  switch (bControlSelector) {

  case FEATURE_VOLUME:
    (haudio->control.data)[0] = (uint16_t) VOL_RES & 0xFF;
    (haudio->control.data)[1] = ((uint16_t) VOL_RES & 0xFF00) >> 8;
    break;

  case FEATURE_GRAPHIC_EQ:
    (haudio->control.data)[0] = 0xff;   // the first 10 bands are supported
    (haudio->control.data)[1] = 0x3;
    (haudio->control.data)[2] = 0;
    (haudio->control.data)[3] = 0;

    for (uint8_t i = 0; i < 10; i++) {
      (haudio->control.data)[i + 4] = 4;    // resolution = 1dB, units here are 0.25dB
    }
    break;

    default:
      sprintf(Utils::UART2_TX_Buffer, ">>AUDIO_REQ_GetResolution unhandle selector\n");
      Utils::getInstance()->console_write(Utils::UART2_TX_Buffer);
    break;

  }

  USBD_CtlSendData(pdev, haudio->control.data, req->wLength);
}



/**
  * @brief  DeviceQualifierDescriptor
  *         return Device Qualifier descriptor
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t *USBD_AUDIO_GetDeviceQualifierDesc(uint16_t *length)
{
  *length = (uint16_t)sizeof(USBD_AUDIO_DeviceQualifierDesc);
  //sprintf(Utils::UART2_TX_Buffer, ">>USBD_AUDIO_GetDeviceQualifierDesc\n");
  //Utils::getInstance()->console_write(Utils::UART2_TX_Buffer);
  return USBD_AUDIO_DeviceQualifierDesc;
}

/**
  * @brief  USBD_AUDIO_RegisterInterface
  * @param  fops: Audio interface callback
  * @retval status
  */
uint8_t USBD_AUDIO_RegisterInterface(USBD_HandleTypeDef *pdev,
                                     USBD_AUDIO_ItfTypeDef *fops)
{
  
  sprintf(Utils::UART2_TX_Buffer, ">>USBD_AUDIO_RegisterInterface\n");
  Utils::getInstance()->console_write(Utils::UART2_TX_Buffer);
  if (fops == NULL)
  {
    return (uint8_t)USBD_FAIL;
  }

  pdev->pUserData = fops;

  sprintf(Utils::UART2_TX_Buffer, "<<USBD_AUDIO_RegisterInterface\n");
  Utils::getInstance()->console_write(Utils::UART2_TX_Buffer);

  return (uint8_t)USBD_OK;
}
 