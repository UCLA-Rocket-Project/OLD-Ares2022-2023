# avionics/flight
This folder includes all Flight Software (FSW), specifically the code/files related to the operation of Arduino boards within the Avionics system.


## Directory Structure

```
avionics/flight
└───bodytube
└───ground
└───nosecone
└───shock
└───libraries.zip
```

### bodytube
This folder includes the ino file for the in-flight Bodytube System.

### ground
This folder includes the ino file for the on-ground Ground Station.

### nosecone
This folder includes the ino file for the in-flight Nosecone System.

### shock
This folder includes the ino file for the in-flight Bodytube Shock System.

### libraries.zip
This zip file includes all libraries needed for compiling the ino files within the sub-directories of this directory. Please refer to [this link](https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries) on details about the installation of Arduino libraries.

## Architecture
WIP

## Implementation Details

### Teensy Boards
All files in this directory were created for Teensy 4.1 board. Flashing to a different board may require some basic modification of the code.

### SDFat Library
In all the ino files, saving data utilizes SDFat library to maximize performance and minimize bottleneck. I advise against using the default Arduino SD library for saving to SD cards. The files assume that the SD card is formatted in EXFat format. Please use the [SD assocation tool](https://www.sdcard.org/downloads/formatter/) for formatting microSD cards. Following SD associations guideline, it is recommended to use microSD of size 64 GB or above for EXFat standard.

### Binary Encoding
When saving data, all ino files in this directory save the data in binary bytes. For example, a 4-byte int would be encoded into a 4-byte char array and is written directly into the SD card. This is to guarantee that the data saved at every loop maintains a constant size and to decrease bottleneck in saving data. This kind of encoding is also used when transferring the data over radio from the nosecone system to the ground system.