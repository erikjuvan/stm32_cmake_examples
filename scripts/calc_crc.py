import zlib
import sys
import crcmod
import binascii
import struct

def msb(val: int, bits: int = 8) -> bool:
    return bool(val & (1 << (bits - 1)))

def embedded_crc(input_data: int, poly: int = 0x04C11DB7, initial_crc: int = 0xFFFFFFFF, sizeof_input: int = 32) -> int:
    msk = 0
    for i in range(sizeof_input):
        msk = (msk << 1) | 1

    crc = initial_crc ^ input_data

    b_index = 0
    while b_index < sizeof_input:
        if msb(crc, sizeof_input):
            crc = ((crc << 1) ^ poly) & msk
        else:
            crc = (crc << 1) & msk

        b_index += 1

    return crc

def calc_embedded_crc(binary, poly: int = 0x04C11DB7, initial_crc: int = 0xFFFFFFFF, sizeof_input: int = 32, ) -> int:
    crc = initial_crc
    words = len(binary) // 4
    for i in range(words):
        chunk = binary[i * 4: i * 4 + 4]
        val = struct.unpack('>I', chunk)[0] # This is because of ClassB code
        crc = embedded_crc(val, poly, crc, sizeof_input)

    return crc

def crc32mpeg2(msg):
    crc = 0xffffffff
    for b in msg:
        crc ^= b << 24
        for _ in range(8):
            crc = (crc << 1) ^ 0x04c11db7 if crc & 0x80000000 else crc << 1
    return crc & 0xffffffff
   
# CRC-32/MPEG-2 (used by add_crc.py)
crc32 = crcmod.mkCrcFun(poly=0x104c11db7, rev=False, initCrc=0xFFFFFFFF, xorOut=0x0)

with open(sys.argv[1], 'rb') as f:
    data = bytearray(f.read())
    data = data[:-4]
    test_data = b'12345678'
    print("add_crc.py-embedded_crc(" + str(test_data) + "): " + str(hex(calc_embedded_crc(test_data))))
    print("add_crc.py-embedded_crc(data): " + str(hex(calc_embedded_crc(data))))
    print("crc32mpeg2(" + str(test_data) + "): " + str(hex(crc32mpeg2(test_data))))
    # It takes too long 
    #print("crc32mpeg2(data): " + str(hex(crc32mpeg2(data))))
    print("crcmod.mkCrcFun(" + str(test_data) + "): " + str(hex(crc32(test_data))))
    print("crcmod.mkCrcFun(data): " + str(hex(crc32(data))))
    print("binascii.crc32(" + str(test_data) + "): " + str(hex(binascii.crc32(test_data))))
    print("binascii.crc32(data): " + str(hex(binascii.crc32(data))))
    print("zlib.crc32(" + str(test_data) + "): " + str(hex(zlib.crc32(test_data))))
    print("zlib.crc32(data): " + str(hex(zlib.crc32(data))))
