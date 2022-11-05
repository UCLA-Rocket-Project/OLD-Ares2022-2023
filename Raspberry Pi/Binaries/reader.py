import serial, time, socket, os
import piplates.DAQC2plate as daq

labels = ['fuel_tank_pt', 'cc_pt', 'low_he_pt', 'lox_tank_pt', 'lox_manifold_pt', 'fuel_manifold_pt', 'high_he_pt', 'lc']
log = open('/home/rocket/logs/pt_loadcell.csv', 'a')
log_raw = open('/home/rocket/logs/pt_loadcell_voltage.csv', 'a')
a = [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0]
b = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]

def millis():
    return int(time.time_ns() / 1000000)

addr = ('127.0.0.1', 2000)
client = socket.socket(family = socket.AF_INET, type = socket.SOCK_DGRAM)

prev = millis()

while True:
    data = []
    try:
        data = daq.getADCall(0)
    except:
        os._exit(1)
    if len(data) == len(labels):
        now = millis()
        log_raw.write(str(now) + ',' + data.join(',') + '\n')
        c_data = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
        for i in range(8):
            c_data[i] = a[i] * data[i] + b[i]
        log.write(str(now) + ',' + c_data.join(',') + '\n')
        if now > prev + 100:
            for l, d in zip(labels, c_data):
                if d != 'null':
                    bytes = (l + ' ' + l + '=' + d + ' ' + str(now * 1000000)).encode()
                    client.sendto(bytes, addr)
            prev = now
