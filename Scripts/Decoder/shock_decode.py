import struct

f = open('FILE0000.CHK', 'rb')
q = 0
while True:
    c = f.read(16)
    q += 1
    if q > 42056225:
        if (len(c) != 16):
            break
        d = (struct.unpack('I', c[0:4])[0], struct.unpack('f', c[4:8])[0] * 100.0 / 1000.0, struct.unpack('f', c[8:12])[0] * 100.0 / 1000.0, struct.unpack('f', c[12:16])[0] * 100.0 / 1000.0)

        if (d[0] == 0):
            break

        j = 0
        for i in d:
            print(i, end = '')
            if j == 3:
                print()
            else:
                print(',', end = '')
            j += 1

    
    """
    if d[3] > 3.0:
        j = 0
        print(d[0], end = '')
        print('\t\t', end = '')
        print(d[0] - p, end = '')
        print('\t\t\t', end = '')
        print(d[3])
        p = d[0]
        
        for i in d:
            print(i, end = '')
            if j == 3:
                print()
            else:
                print(',', end = '')
            j += 1
    """
    