#include <usbgen_src.h>
#include "usbd_desc.h"
#include "usbd_audio.h"

using namespace usb;
using namespace usb::audio;

extern "C" constexpr auto USBDeviceDesc = make_device(
    device::bcdUSB(0x0200),
    device::idVendor(USBD_VID),
    device::idProduct(USBD_PID_FS),
    device::bMaxPacketSize(USB_MAX_EP0_SIZE),
    device::bcdDevice(0x0200),
    device::iManufacturer(1),
    device::iProduct(2),
    device::iSerialNumber(3),
    device::bNumConfigurations(1),
    make_configuration(
        configuration::bConfigurationValue(1),
        configuration::bNumInterfaces(2),
        configuration::bMaxPower(80),
        configuration::bmAttributes(0x80),
        make_interface(
            interface::bInterfaceNumber(0),
            interface::bAlternateSetting(0),
            interface::bNumEndpoints(0),
            interface::bInterfaceClass(Audio),
            interface::bInterfaceSubClass(SubclassAudioControl),
            interface::bInterfaceProtocol(ProtocolUndefined),
            interface::iInterface(0)
            ),
        make_control_header<1>(
            header::bcdADC(0x0100),
            header::baInterfaceNr<1>(),
          
                 make_input_terminal(
                    input_terminal::bTerminalID(AUDIO_TERMINAL_MIC_IN),
                    input_terminal::wTerminalType(TerminalMicrophone),
                    input_terminal::bAssocTerminal(0),
                    input_terminal::bNrChannels(2),
                    input_terminal::wChannelConfig(LeftFront | RightFront)
                     
                ),
                
                 make_output_terminal(
                    output_terminal::bTerminalID(AUDIO_TERMINAL_USB_IN),
                    output_terminal::bSourceID(AUDIO_TERMINAL_FEATURE_MIC_IN),
                    output_terminal::wTerminalType(USBStreaming),
                    output_terminal::bAssocTerminal(0)
                    
                ) ,

                  make_feature_unit<uint16_t, 2>(
                feature_unit::bUnitID(AUDIO_TERMINAL_FEATURE_MIC_IN),
                feature_unit::bSourceID(AUDIO_TERMINAL_MIC_IN),
                feature_unit::bmaControls<Mute, Volume, Volume >()
                
                )   
              
             ),
        make_interface(
            interface::bInterfaceNumber(AUDIO_INTERFACE_ID_MIC),
            interface::bAlternateSetting(0),
            interface::bNumEndpoints(0),
            interface::bInterfaceClass(Audio),
            interface::bInterfaceSubClass(SubclassAudioStreaming),
            interface::bInterfaceProtocol(ProtocolUndefined)
        ),
        make_interface(
            interface::bInterfaceNumber(AUDIO_INTERFACE_ID_MIC),
            interface::bAlternateSetting(1),
            interface::bNumEndpoints(1),
            interface::bInterfaceClass(Audio),
            interface::bInterfaceSubClass(SubclassAudioStreaming),
            interface::bInterfaceProtocol(ProtocolUndefined)
        ),
       
       
         make_streaming_interface(
            streaming_interface::bTerminalLink(AUDIO_TERMINAL_USB_IN),
            streaming_interface::bDelay(1),
            streaming_interface::wFormatTag(PCM)
            ),
        make_format_type_i<4>(
            format_type_i::bNrChannels(2),
            format_type_i::bSubframeSize(2),
            format_type_i::bBitResolution(16),
            format_type_i::tSamFreq<16000, 22050, 44100, 48000>()
            
            ),
        make_iso_data_endpoint(
            endpoint::bEndpointAddress(AUDIO_MIC_IN_USB_EP),
            endpoint::wMaxPacketSize(192),
            endpoint::bmAttributes((Isochronous | Asynchronous | DataEndpoint)),
            endpoint::bInterval(1)
            
        ),
        make_as_iso_data_endpoint(

            endpoint::bmAttributes(1)
        )        
        
    ));

auto &Config = USBDeviceDesc.sub<0>();

extern "C" constexpr auto *USBDevice = USBDeviceDesc.pointer();
extern "C" constexpr size_t USBDeviceSize = sizeof(USBDeviceDesc);

extern "C" constexpr auto *USBConfiguration = Config.pointer();
extern "C" constexpr auto USBConfigurationSize = sizeof(Config);

extern "C" constexpr auto USBTest1StringData = make_string(u"test");
extern "C" constexpr auto USBTest2StringData = make_string({(char16_t)1033});