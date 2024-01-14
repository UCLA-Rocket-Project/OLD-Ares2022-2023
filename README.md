# Ares2022-2023
Central Repository for Ares Software for the 2022-2023 School Year. This repository contains all software for Avionics and Ground Systems, as well as any additional code/scripts used to support the development, deployment, and/or operation of the software. Additionally, select data and documentation is also version-controlled in this repository, enabling a "one-stop-shop" for everything of interest.


## Base-Level Directory Structure

```
AresSoftware
└───avionics
└───data
└───ground_system
└───scripts
```

### avionics
Contains all Avionics-related software, including Arduino microcontroller source code and various scripts used for operation.

### ground_systems
Contains all Ground Systems-related software, including all software used to enable our Raspberry Pi server, as well as Arduino microcontroller source code and various scripts used for software deployment and system operation.

### data
Contains all "important" data that we desire to version-control. This could include logged data from tests and/or full operations (e.g. static fires, cold flows, launches, etc.).

### scripts
Contains overall GENERAL scripts, primarily for supporting nominal software development.


## Overall Relevant Electronics System Architecture
![rocket drawio](https://user-images.githubusercontent.com/31735483/192203986-2f849688-51ab-46fd-9736-e6a44f8085f0.png)

The architecture diagram omits individual sensors and specific components and only gives a high-level overview of the system. For a more specific component-level description, please refer to the documentation within individual folders.
