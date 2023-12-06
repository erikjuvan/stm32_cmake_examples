import sys
from elftools.elf.elffile import ELFFile
from struct import pack
import crcmod

# project name as first argument
proj_name = sys.argv[1]

bin_file_name = proj_name  + ".bin"
elf_file_name = proj_name  + ".elf"

def main(file_elf, file_binary): #, data):
    f = open(file_elf, 'r+b')
    e = ELFFile(f)
    isr_vector = e.get_section_by_name('.isr_vector')
    safe_crc   = e.get_section_by_name('.safe_crc')

    # Application sections
    offset_to_value = safe_crc.header.sh_size - 4
    app_bin_start   = isr_vector.header.sh_addr
    app_bin_end     = safe_crc.header.sh_addr + offset_to_value
    app_len         = app_bin_end - app_bin_start

    app_bin_end_dec = app_bin_end - 1
    
    # length must be divisible by (4) and start aligned(4)
    if app_len % 4 or app_bin_start % 4:
        raise ValueError('Is something wrong with linker file? App length is not divisible by 4.')
    
    # Info
    print(f"start at offset: 0x{app_bin_start:0{8}x}")
    print(f"end   at offset: 0x{app_bin_end_dec:0{8}x}")
    print(f'CRC calc over bytes: {app_len}')

    # ELF sections where crc value resides
    flash_sh_end = safe_crc.header.sh_offset + offset_to_value

    # Calculate CRC-32/MPEG-2 (used by STM HW)
    with open(file_binary, "rb") as fb:
        crc32_func = crcmod.mkCrcFun(poly=0x104c11db7, rev=False, initCrc=0xFFFFFFFF, xorOut=0x0)    
        data = bytearray(fb.read())
        data = data[:-4] # don't use the last 4 bytes which is the location where crc will be saved
        crc = crc32_func(data)
        # Write CRC32
        crc_bytes = pack("I", crc)
        f.seek(flash_sh_end)
        f.write(crc_bytes)
        # Print computed CRC 
        print(f'CRC is: 0x{crc:0{8}x} at address: 0x{app_bin_end:0{8}x}')

    # Close files
    f.close()    

if __name__ == "__main__":
#    if len(sys.argv) != 3:
#        raise ValueError('Expected 2 args: *.elf *.bin !')
    print('########################################################')
    print('Calculating CRC for .elf file')

    main(elf_file_name, bin_file_name)
    
    print('########################################################\n')
    