# server/configs
This folder includes all config files for prebuilt executables.


## Directory Structure

```
server/configs
└───grafana
└───telegraf
|   dhcpcd.conf
```

### grafana
Files for Grafana config. For installation, please refer to [this link](https://grafana.com/docs/grafana/latest/setup-grafana/configure-grafana/). This config only changes one parameter related to allowing [unsafe HMTL scripts] (https://github.com/grafana/grafana/issues/15647). This parameter must be enabled to allow Javascript to run in Grafana HTML widgets.

### telegraf
Files for Telegraf config. For installation and more details, please refer to [this link](https://docs.influxdata.com/telegraf/v1.24/configuration/). 

Currently, this file configures Telegraf to have one input of UDP socket running at loopback IP with port 2000 and two outputs -- one for Grafana and one for InfluxDB. The InfluxDB config has been commented due to some issues with the bucket becoming corrrupted occassionally when the Raspberry Pi is shut down abnormally (by pulling power cord).

All data for output and input uses [InfluxDB Line Protocol](https://docs.influxdata.com/influxdb/v1.8/write_protocols/line_protocol_tutorial/).

### dhcpcd.conf
Configuration file for DHCP. We use this file to statically assign an IP address for our RPi when on the dedicated URP Ground Systems LAN.


## Architecture
WIP