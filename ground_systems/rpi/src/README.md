# Binaries
This folder includes all custom binary files and source code for Raspberry Pi.


## Directory Structure

```
Binaries
└───ADC Source Code
|   app.py
|   image.py
|   pnid.png
|   redirect.py
|   tc.py
```

### ADC Source Code
This folder includes the C++ files and related files for compiling the ADC binary.

### app.py
This file is the source code for the controls server. The controls server utilizes the [Flask](https://flask.palletsprojects.com/en/2.2.x/) framework. Whenever a controls url is opened, the controls data is pinged to the controls Arduino via USB. When the Arduino acknowledges the data, the data is then pinged to Telegraf.

### image.py
This file is the source code for a simple server that hosts the image for pnid diagram. The pnid diagram needs to be hosted as a website due to the limitations of the Grafana extensions that allow overlaying values on top of an image.

### pnid.png
This is the pnid diagram supplied by the propulsion team.

### redirect.py
This file handles redirection of urls. Because entering in port numbers in browser url can be confusing, this script hosts a server that redirects certain urls under the rocket.local domain to actual ip address and port number.

### tc.py
This script takes inputs from the thermocouple Arduino via USB and streams it to Telegraf and logs it to a text file.


## Architecture
WIP
