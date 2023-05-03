# rpi/src
This folder includes all the actual source code files for the necessary Ground Software (GSW) running on the Ground Systems' Raspberry Pi.

## Directory Structure

```
rpi/src
└───adc
└───environments
└───server
└───thermocouple
```

### adc
This folder includes all source code supporting the compilation of the ADC binary (including driver code, Makefiles, main operation code, etc.).

### environments
This folder includes Linux-related environment files that allow running programs as system services on boot.

### server
This folder includes all scripts/files needed for the operation of the Python Flask server, as well as configurations for its related modules (i.e. Grafana, Telegraf).