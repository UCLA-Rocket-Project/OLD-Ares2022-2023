import struct

f = open('2.txt', 'rb')
while True:
    c = f.read(32)
    if c == '':
        break
    print(struct.unpack('I', c[0:4])[0], end = '')
    print(',', end = '')
    print(struct.unpack('i', c[4:8])[0], end = '')
    print(',', end = '')
    print(struct.unpack('d', c[8:16])[0], end = '')
    print(',', end = '')
    print(struct.unpack('d', c[16:24])[0], end = '')
    print(',', end = '')
    print(struct.unpack('d', c[24:32])[0], end = '')
    print()