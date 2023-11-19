# lukit-tone-generator
This is a firmware for the STM32F4 microprocessor family that generates tones and sends them to the PC using the audio USB microphone library. The code is written in C/C++ and is built, uploaded, and debugged using the PlatformIO IDE.

The code for this project is hosted on two GitHub repositories:

. lukit-tone-generator (this repository) contains the core firmware code.
. lukit-usb-tonegen contains the USB drivers needed to communicate with the firmware.
# Set up
To use this firmware, you'll need:

- An STM32F4 microprocessor or development board
- A computer running the PlatformIO IDE
# Installation
To install this firmware, first clone both repositories to your local machine. Then, copy the contents of the lukit-usb-tonegen repository into the lukit-tone-generator repository. Finally, open the lukit-tone-generator folder in PlatformIO and build the code. Upload the resulting binary to your microprocessor or development board using PlatformIO's upload feature.

# Usage
Once you've installed the firmware, you can use it to generate tones and send them to your PC via USB. To use the firmware, simply connect your microprocessor or development board to your PC using a USB cable and open a terminal program (such as screen or minicom). Then, send commands to the firmware over the serial connection to generate tones.

For more information on using this firmware or troubleshooting issues, please contact Luis Coelho at luis.coelho.720813@gmail.com.

# License
This firmware is licensed under the MIT License. See LICENSE.txt for more information.

 

 
