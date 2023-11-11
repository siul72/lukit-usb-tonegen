#ifndef __USBD_DESC__C__
#define __USBD_DESC__C__

#ifdef __cplusplus
 extern "C" {
#endif

 
#include "usbd_def.h"
 
#define         DEVICE_ID1          (UID_BASE)
#define         DEVICE_ID2          (UID_BASE + 0x4)
#define         DEVICE_ID3          (UID_BASE + 0x8)

#define  USB_SIZ_STRING_SERIAL       0x1A
 
extern USBD_DescriptorsTypeDef FS_Desc;

#define USBD_VID     0x16c0
#define USBD_LANGID_STRING     1033
#define USBD_MANUFACTURER_STRING     "Lukit"
#define USBD_PID_FS     0x27e0
#define USBD_PRODUCT_STRING_FS     "Lukit Telluino"
#define USBD_CONFIGURATION_STRING_FS     "AUDIO Config"
#define USBD_INTERFACE_STRING_FS     "AUDIO Interface"
 

#ifdef __cplusplus
}
#endif

#endif /* __USBD_DESC__C__ */

