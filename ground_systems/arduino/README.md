# ground_systems/arduino
This folder includes all Ground Software (GSW), specifically the code/files related to the operation of Arduino boards within the Ground Systems.


## Directory Structure

```
ground_systems/arduino
└───controls_pad
└───controls_switch_box
└───thermocouples
└───libraries.zip
```

### controls_pad
This folder includes the ino file for the Arduino board within the on-pad Controls Pad Subsystem.

### controls_switch_box
This folder includes the ino file for the Arduino-compatible board (ESP32) within the in-bunker Switch Box Subsystem. Uploading to an ESP32 board requires a special process. For more information, refer to [this article](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/). NOTE: This resource may not actually work, since it's been a while since a re-upload was required. However, to perhaps help with finding the correct resource, we're currently utilizing the OLIMEX ESP32-PoE model (see [this pinout diagram](https://www.olimex.com/Products/IoT/ESP32/ESP32-POE/resources/ESP32-POE-GPIO.png)).

### thermocouples
This folder includes the ino file for the Arduino-compatible board (Teensy 4.1) within the separate on-pad Thermocouple Subsystem.

### libraries.zip
This zip file includes all libraries needed for compiling the ino files within the sub-directories of this directory. Please refer to [this link](https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries) on details about the installation of Arduino libraries.

## Architecture
WIP

## Implementation Details
### Controls Data Integrity
When sending data to and from controls Arduino, it goes through very basic sanity checks to ensure a low-probability of bit corruption. Details WIP.

### Switch Box Update Frequency
The switch box sends update to the server every 0.5 seconds or whenever a change in a switch state is detected.

## Possible Future Extensions
### Switch Box Revamp
We never got the Ethernet to work on the ESP32 on the switch box. We would advise looking into other Arduino-comptabible boards (or Ethernet breakout boards usable for ESP32s). If we want to be fancy, we can try putting a full Raspberry Pi into the switch box, which will allow you to run more interesting programs as well as display the DAQ webpage if you connect a monitor to the Raspberry Pi.