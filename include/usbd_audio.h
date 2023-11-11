/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_AUDIO_H
#define __USB_AUDIO_H

#ifdef __cplusplus
extern "C" {
#endif
 
#include  "usbd_ioreq.h"
 
#ifndef USBD_AUDIO_FREQ
  #define USBD_AUDIO_FREQ                               48000U
#endif /* USBD_AUDIO_FREQ */

#ifndef USBD_MAX_NUM_INTERFACES
#define USBD_MAX_NUM_INTERFACES                       1U
#endif /* USBD_AUDIO_FREQ */

#ifndef AUDIO_HS_BINTERVAL
#define AUDIO_HS_BINTERVAL                            0x01U
#endif /* AUDIO_HS_BINTERVAL */

#ifndef AUDIO_FS_BINTERVAL
#define AUDIO_FS_BINTERVAL                            0x01U
#endif /* AUDIO_FS_BINTERVAL */

#define AUDIO_OUT_EP1                                 0x01U
#define AUDIO_IN_EP1                                  0x81U
#define AUDIO_OUT_EP2                                 0x02U
#define AUDIO_IN_EP2                                  0x82U

#define AUDIO_MIC_IN_USB_EP 0x81
 
#define AUDIO_INTERFACE_DESC_SIZE                     0x09U
#define AUDIO_INTERFACE_SPECIFIC_DESC_SIZE            0x0AU
#define USB_AUDIO_DESC_SIZ                            0x09U
#define AUDIO_STANDARD_ENDPOINT_DESC_SIZE             0x09U
#define AUDIO_STREAMING_ENDPOINT_DESC_SIZE            0x07U

#define AUDIO_DESCRIPTOR_TYPE                         0x21U
#define USB_DEVICE_CLASS_AUDIO                        0x01U
#define AUDIO_SUBCLASS_AUDIOCONTROL                   0x01U
#define AUDIO_SUBCLASS_AUDIOSTREAMING                 0x02U
#define AUDIO_PROTOCOL_UNDEFINED                      0x00U
#define AUDIO_STREAMING_GENERAL                       0x01U
#define AUDIO_STREAMING_FORMAT_TYPE                   0x02U

#define AUDIO_TERMINAL_USB_STREAMING                  0X0101U
#define AUDIO_TERNIMAL_PHONE_LINE_IN                  0x501U

/* Audio Descriptor Types */
#define AUDIO_INTERFACE_DESCRIPTOR_TYPE               0x24U
#define AUDIO_ENDPOINT_DESCRIPTOR_TYPE                0x25U

/* Audio Control Interface Descriptor Subtypes */
#define AUDIO_CONTROL_HEADER                          0x01U
#define AUDIO_CONTROL_INPUT_TERMINAL                  0x02U
#define AUDIO_CONTROL_OUTPUT_TERMINAL                 0x03U
#define AUDIO_CONTROL_FEATURE_UNIT                    0x06U

#define AUDIO_INPUT_TERMINAL_DESC_SIZE                0x0CU
#define AUDIO_OUTPUT_TERMINAL_DESC_SIZE               0x09U
#define AUDIO_STREAMING_INTERFACE_DESC_SIZE           0x07U

#define AUDIO_CONTROL_MUTE                            0x0001U

#define AUDIO_FORMAT_TYPE_I                           0x01U
#define AUDIO_FORMAT_TYPE_III                         0x03U

#define AUDIO_ENDPOINT_GENERAL                        0x01U

#define AUDIO_REQ_GET_CUR                             0x81U
#define AUDIO_REQ_GET_MIN                             0x82
#define AUDIO_REQ_GET_MAX                             0x83
#define AUDIO_REQ_GET_RES                             0x84
#define AUDIO_REQ_SET_CUR                             0x01
 

#define AUDIO_OUT_STREAMING_CTRL                      0x02U

#define AUDIO_OUT_TC                                  0x01U
#define AUDIO_IN_TC                                   0x02U

#define OSA_BUF_SIZE 16
#define AUDIO_OUT_PACKET                              (uint16_t)(((USBD_AUDIO_FREQ * 2U * 2U) / 1000U))
#define AUDIO_IN_PACKET                              (uint16_t)(((USBD_AUDIO_FREQ * 2U * 2U) / 1000U))
#define AUDIO_DEFAULT_VOLUME                          70U

#define AUDIO_CTRL_REQ_SET_CUR_MUTE      0x00
#define AUDIO_CTRL_REQ_SET_CUR_VOLUME    0x01
#define AUDIO_CTRL_REQ_SET_CUR_EQUALIZER 0x02

#define VOL_MIN                                       0xb000    // -80dB (1 == 1/256dB)
#define VOL_RES                                       128       // 0.5dB (1 == 1/256dB)
#define VOL_MAX                                       9216      // 36dB (1 == 1/256dB)

#define FEATURE_MUTE       0x01
#define FEATURE_VOLUME     0x02
#define FEATURE_BASS       0x04
#define FEATURE_MID        0x08
#define FEATURE_TREBLE     0x10
#define FEATURE_GRAPHIC_EQ 0x20
#define FEATURE_AUTO_GAIN  0x40
#define FEATURE_DELAY      0x80



/* Buffering state definitions */
typedef enum {
  STATE_USB_WAITING_FOR_INIT = 0, STATE_USB_IDLE = 1, STATE_USB_REQUESTS_STARTED = 2, STATE_USB_BUFFER_WRITE_STARTED = 3,
} AUDIO_StatesTypeDef;

/* Number of sub-packets in the audio transfer buffer.*/
#define AUDIO_IN_PACKET_NUM                            6
/* Number of sub-packets in the audio transfer buffer. You can modify this value but always make sure
  that it is an even number and higher than 3 */
#define AUDIO_OUT_PACKET_NUM                          6
/* Total size of the audio transfer buffer */
#define AUDIO_TOTAL_BUF_SIZE                          ((uint16_t)(AUDIO_OUT_PACKET * AUDIO_OUT_PACKET_NUM))

//INTERFACE IDS
#define AUDIO_INTERFACE_ID_MIC  1

// TERMINAL IDS
#define AUDIO_TERMINAL_MIC_IN                             1
#define AUDIO_TERMINAL_USB_IN                             2
#define AUDIO_TERMINAL_FEATURE_MIC_IN                     3

/* Audio Commands enumeration */
typedef enum
{
  AUDIO_CMD_START = 1,
  AUDIO_CMD_PLAY,
  AUDIO_CMD_STOP,
} AUDIO_CMD_TypeDef;


typedef enum
{
  AUDIO_OFFSET_NONE = 0,
  AUDIO_OFFSET_HALF,
  AUDIO_OFFSET_FULL,
  AUDIO_OFFSET_UNKNOWN,
} AUDIO_OffsetTypeDef;
/**
  * @}
  */


/** @defgroup USBD_CORE_Exported_TypesDefinitions
  * @{
  */
typedef struct
{
  uint8_t cmd;
  uint8_t data[USB_MAX_EP0_SIZE];
  uint8_t len;
  uint8_t unit;
} USBD_AUDIO_ControlTypeDef;


typedef struct
{
  uint32_t alt_setting;
  uint8_t buffer[AUDIO_TOTAL_BUF_SIZE];
  AUDIO_OffsetTypeDef offset;
  uint8_t rd_enable;
  uint16_t rd_ptr;
  uint16_t wr_ptr;
  USBD_AUDIO_ControlTypeDef control;
  uint8_t channels;
  uint32_t frequency;
  __IO int16_t timeout;
  uint16_t buffer_length;
  uint16_t dataAmount;
  uint16_t paketDimension;
  uint8_t state;
  uint8_t upper_treshold;
  uint8_t lower_treshold;
  //uint8_t *buffer;

} USBD_AUDIO_HandleTypeDef;


typedef struct
{
  int8_t (*Init)(uint32_t AudioFreq, uint32_t Volume, uint32_t options);
  int8_t (*DeInit)(uint32_t options);
  int8_t (*AudioCmd)(uint8_t *pbuf, uint32_t size, uint8_t cmd);
  int8_t (*VolumeCtl)(uint8_t vol);
  int8_t (*MuteCtl)(uint8_t cmd);
  int8_t (*PeriodicTC)(uint8_t *pbuf, uint32_t size, uint8_t cmd);
  int8_t (*GetState)(void);
  int8_t (*Record)(void);
  int8_t (*Stop)(void);
  int8_t (*Pause)(void);
  int8_t (*Resume)(void);
  int8_t (*CommandMgr)(uint8_t cmd);
} USBD_AUDIO_ItfTypeDef;
 
 

extern USBD_ClassTypeDef USBD_AUDIO;
#define USBD_AUDIO_CLASS &USBD_AUDIO
 
uint8_t USBD_AUDIO_RegisterInterface(USBD_HandleTypeDef *pdev,
                                     USBD_AUDIO_ItfTypeDef *fops);

void USBD_AUDIO_Sync(USBD_HandleTypeDef *pdev, AUDIO_OffsetTypeDef offset);
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif  /* __USB_AUDIO_H */
 