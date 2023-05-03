import struct

f = open('9.txt', 'rb')
while True:
    c = f.read(42)
    if c == '':
        break
    try:
        print(struct.unpack('I', c[0:4])[0], end = '')
        print(',', end = '')
        print(struct.unpack('i', c[4:8])[0], end = '')
        print(',', end = '')
        print(struct.unpack('i', c[8:12])[0], end = '')
        print(',', end = '')
        print(struct.unpack('i', c[12:16])[0], end = '')
        print(',', end = '')
        print(struct.unpack('H', c[16:18])[0], end = '')
        print(',', end = '')
        print(struct.unpack('d', c[18:26])[0], end = '')
        print(',', end = '')
        print(struct.unpack('d', c[26:34])[0], end = '')
        print(',', end = '')
        print(struct.unpack('d', c[34:42])[0], end = '')
        print()
    except:
        break