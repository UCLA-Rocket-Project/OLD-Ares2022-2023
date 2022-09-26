# Ground Systems
This folder includes all code/files related to the operation of Arduino boards within the ground systems.


## Directory Structure

```
Ground Systems
└───Controls
└───Switch Box
```

### Controls
This folder includes the ino file for the controls Arduino.

### Switch Box
This folder includes the ino file for the switch box Arduino-compatible board, which currently is an ESP32.


## Architecture
WIP


## Implementation Details
### Controls Data Integrity
When sending data to and from controls Arduino, it goes through very basic sanity checks to ensure a low-probability of bit corruption. Details WIP.

### Switch Box Update Frequency
The switch box sends update to the server every 0.5 seconds or whenever a change in a switch state is detected.


## Possible Future Extensions
### Switch Box Revamp
I never got the Ethernet to work on the ESP32 on the switch box. I would advise looking into other Arduino-comptabible boards (or Ethernet breakout boards usable for ESP32s). If you want to be fancy, you can try putting a full Raspberry Pi into the switch box, which will allow you to run more interesting programs as well as display the DAQ webpage if you connect a monitor to the Raspberry Pi.
