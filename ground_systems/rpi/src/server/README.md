# server
This folder includes all files related to the operation of the Ground Systems' Raspberry Pi DAQ and Controls Server.


## Directory Structure

```
server
└───configs
└───img
|   app.py
|   image.py
|   redirect.py
|   tc.py
```

### configs
This folder includes the C++ files and related files for compiling the ADC binary.

### img
This folder contains all the images/assets needed by our server (including a pnid diagram supplied by the propulsion team).

### app.py
This file is the source code for the controls server. The controls server utilizes the [Flask](https://flask.palletsprojects.com/en/2.2.x/) framework. Whenever a controls url is opened, the controls data is pinged to the controls Arduino via USB. When the Arduino acknowledges the data, the data is then pinged to Telegraf.

### image.py
This file is the source code for a simple server that hosts the image for pnid diagram. The pnid diagram needs to be hosted as a website due to the limitations of the Grafana extensions that allow overlaying values on top of an image.

### redirect.py
This file handles redirection of urls. Because entering in port numbers in browser url can be confusing, this script hosts a server that redirects certain urls under the rocket.local domain to actual ip address and port number.

### tc.py
This folder includes all supporting scripts for reading/saving/posting/streaming temperature data from the Thermocouple Subsystem through USB serial to the locally-hosted server.


## Architecture
WIP