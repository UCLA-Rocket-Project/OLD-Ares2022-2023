# AresSoftware
This is a central repository for Ares software. This repository contains all files for Raspberry Pi, Arduino-compatible boards, and post-processing.


## Directory Structure

```
AresSoftware
└───Arduino
└───Raspberry Pi
└───Scripts
│   README.md
```

### Arduino
This folder includes all ino files and Arduino libraries needed for ground and avionics Arduino systems. For simplicity, within this repository, all Arduino-compatible boards may be referred to as Arduino. If a specific board is required for operation, this will be specified within the documentation in the same folder as the ino file.

### Raspberry Pi
This folder includes all source code and operating system files for ground Raspberry Pi. This includes config files for Grafana, Influx DB, and Telegraf.

### Scripts
This folder includes files for post-processing the encoded data.


## Overall Architecture
![rocket drawio](https://user-images.githubusercontent.com/31735483/192203986-2f849688-51ab-46fd-9736-e6a44f8085f0.png)

The architecture diagram omits individual sensors and specific components and only gives a high-level overview of the system. For a more specific component-level description, please refer to the documentation within individual folders.
