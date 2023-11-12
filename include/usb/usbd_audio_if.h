#ifndef __USBD_AUDIO_IF_H__
#define __USBD_AUDIO_IF_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usb/usbd_audio.h"
 
/** AUDIO_IF Interface callback. */
extern USBD_AUDIO_ItfTypeDef USBD_AUDIO_fops;
 
void TransferComplete_CallBack_FS(void);
void HalfTransfer_CallBack_FS(void);
 
#ifdef __cplusplus
}
#endif

#endif /* __USBD_AUDIO_IF_H__ */
