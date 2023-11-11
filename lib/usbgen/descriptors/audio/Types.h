//
// Created by Pavel Prokofyev on 2019-01-05.
//

#ifndef __USBGEN_DESCRIPTORS_AUDIO_TYPES__
#define __USBGEN_DESCRIPTORS_AUDIO_TYPES__

namespace usb {
namespace audio {

enum : uint8_t {
  Undefined = 0x00
};

enum ControlInterfaceSubtype : uint8_t {
  Header = 0x01,
  InputTerminal = 0x02,
  OutputTerminal = 0x03,
  MixerUnit = 0x04,
  SelectorUnit = 0x05,
  FeatureUnit = 0x06,
  ProcessingUnit = 0x07,
  ExtensionUnit = 0x08
};

enum StreamingInterfaceSubtype : uint8_t {
  ASGeneral = 0x01,
  FormatType = 0x02,
  FormatSpecific = 0x03,
};

enum StreamingEndpointSubtype : uint8_t {
  EPGeneral = 0x01,
};

enum UsbTerminalType : uint16_t {
USBUndefined = 0x0100,
//I/O USB Terminal, undefined Type.
USBStreaming = 0x0101,
//I/O A Terminal dealing with a signal carried over an endpoint in an AudioStreaming
//interface. The AudioStreaming interface
//descriptor points to the associated
//Terminal through the bTerminalLink field.
USBVendorSpecific = 0x01FF,
// I/O A Terminal dealing with a signal carried
//over a vendor-specific interface. The
//vendor-specific interface descriptor must
//contain a field that references the
//Terminal
};

enum InputTerminalType : uint16_t {
  // Input Terminal, undefined Type.
  InputUndefined = 0x0200,

  // A generic microphone that does not fit under any of the other
  // classifications.
  TerminalMicrophone = 0x0201,

  // A microphone normally placed on the desktop or integrated into the monitor.
  TerminalDesktopMicrophone = 0x0202,

  // A head-mounted or clip-on microphone.
  TerminalPersonalMicrophone = 0x0203,

  // A microphone designed to pick up voice from more than one speaker at
  // relatively long ranges.
  TerminalOmniDirectionalMicrophone = 0x0204,

  // An array of microphones designed for directional processing using
  // host-based signal processing algorithms.
  TerminalMicrophoneArray = 0x0205,

  // An array of microphones with an embedded signal processor.
  TerminalProcessingMicrophoneArray = 0x0206
};

enum OutputTerminalType : uint16_t {
  // Input Terminal, undefined Type.
 OutUndefined = 0x0300,
 Speaker =  0x0301,
 // O A generic speaker or set of speakers that does not fit under any of the other classifications.
Headphones = 0x0302, 
// O A head-mounted audio output device.
HeadMountedDisplayAudio = 0x0303,
//O The audio part of a VR head mounted display. The Associated Interfaces descriptor can be used to reference the
//HID interface used to report the position and orientation of the HMD.
DesktopSpeaker = 0x0304,
// O Relatively small speaker or set of speakers normally placed on the desktop or integrated into the monitor. These
//speakers are close to the user and have limited stereo separation.
RoomSpeaker = 0x0305,
// O Larger speaker or set of speakers that are heard well anywhere in the room.
CommunicationSpeaker = 0x0306,
// O Speaker or set of speakers designed for voice communication.
LowFrequencyEffectsSpeaker = 0x0307
// O Speaker designed for low frequencies (subwoofer). Not capable of reproducing speech or music.
};

enum ChannelConfig : uint16_t {
  None = 0,
  LeftFront = (1UL << 0),
  RightFront = (1UL << 1),
  CenterFront = (1UL << 2),
  LowFrequencyEnhancement = (1UL << 3),
  LeftSurround = (1UL << 4),
  RightSurround = (1UL << 5),
  LeftCenter = (1UL << 6),
  RightCenter = (1UL << 7),
  Surround = (1UL << 8),
  SideLeft = (1UL << 9),
  SideRight = (1UL << 10),
  Top = (1UL << 11)
};

enum FeatureUnitControls : uint16_t {
  Mute = (1UL << 0),
  Volume = (1UL << 1),
  Bass = (1UL << 2),
  Mid = (1UL << 3),
  Treble = (1UL << 4),
  GraphicEqualizer = (1UL << 5),
  AutomaticGain = (1UL << 6),
  Delay = (1UL << 7),
  BassBoost = (1UL << 8),
  Loudness = (1UL << 9)
};

enum EndPointAttributes : uint8_t {
 
Control =  0x00,
Isochronous =  0x01,
Bulk = 0x02 ,
Interrupt = 0x03 ,
 
NoSynchonisation = (0UL << 2),
Asynchronous = (1UL << 2),
Adaptive = (2UL << 2),
Synchronous = (3UL << 2),

DataEndpoint = (0UL << 4),
FeedbackEndpoint = (1UL << 4),
ExplicitFeedbackDataEndpoint = (2UL << 4)
 
};
/* 
EndPoint bmAttributes
Bits 0..1 Transfer Type
00 = Control
01 = Isochronous
10 = Bulk
11 = Interrupt
Bits 2..7 are reserved. If Isochronous endpoint,
Bits 3..2 = Synchronisation Type (Iso Mode)
00 = No Synchonisation
01 = Asynchronous
10 = Adaptive
11 = Synchronous
Bits 5..4 = Usage Type (Iso Mode)
00 = Data Endpoint
01 = Feedback Endpoint
10 = Explicit Feedback Data Endpoint
11 = Reserved */

enum InterfaceSubclass : uint8_t {
  SubclassAudioControl = 0x01,
  SubclassAudioStreaming = 0x02,
  SubclassMidiStreaming = 0x03
};

enum InterfaceProtocol : uint8_t { ProtocolUndefined = 0x00 };

enum DescriptorType : uint8_t {
  AudioDescriptorUndefined = 0x20,
  AudioDevice = 0x21,
  AudioConfiguration = 0x22,
  AudioString = 0x23,
  AudioInterface = 0x24,
  AudioEndpoint = 0x25,
};

enum DataFormatType : uint8_t {
  FormatTypeI = 0x01,
  FormatTypeII = 0x02,
  FormatTypeIII = 0x03
};

enum DataFormatTypeI : uint16_t {
  TypeIUndefined = 0x0000,
  PCM = 0x0001,
  PCM8 = 0x0002,
  IEEE_FLOAT = 0x0003,
  ALAW = 0x0004,
  MULAW = 0x0005,
};

} // namespace audio
} // namespace usb

#endif //__USBGEN_DESCRIPTORS_AUDIO_TYPES__
