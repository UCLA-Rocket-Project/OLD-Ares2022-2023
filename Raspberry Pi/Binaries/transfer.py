import serial, time, socket, os

port1 = ''
port2 = ''
labels1 = ['lox_fill_pt', 'fuel_tank_pt', 'cc_pt', 'low_he_pt', 'lox_tank_pt', 'lox_manifold_pt', ]
labels2 = ['pneumatics_pt', 'fuel_manifold_pt', 'high_he_pt', 'lc']
log = open('/home/rocket/logs/pt_loadcell.csv', 'a')

def millis():
    return int(time.time_ns() / 1000000)

while True:
    try:
        serial_port1 = serial.Serial(port1, 9600, timeout=0.1)
        serial_port2 = serial.Serial(port2, 9600, timeout=0.1)
    except:
        continue
    finally:
        break

addr = ('127.0.0.1', 2000)
client = socket.socket(family = socket.AF_INET, type = socket.SOCK_DGRAM)

prev = millis()

while True:
    try:
        line1 = serial_port1.readline()
        line2 = serial_port2.readline()
    except:
        os._exit(1)
    line1 = line1.decode()
    line2 = line2.decode()
    data1 = line1[:-2].split(',')
    data2 = line2[:-2].split(',')
    if len(data1) == len(labels1):
        now = millis()
        log.write(str(now) + ',' + line1)
        if now > prev + 500:
            for l, d in zip(labels1, data1):
                if d != 'null':
                    bytes = (l + ' ' + l + '=' + d + ' ' + str(now * 1000000)).encode()
                    client.sendto(bytes, addr)
            prev = now
    if len(data2) == len(labels2):
        now = millis()
        log.write(str(now) + ',' + line2)
        if now > prev + 500:
            for l, d in zip(labels2, data2):
                if d != 'null':
                    bytes = (l + ' ' + l + '=' + d + ' ' + str(now * 1000000)).encode()
                    client.sendto(bytes, addr)
            prev = now
