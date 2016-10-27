from pwn import *

with open('bin','w') as f:
    f.write(p32(0x001f0004) + 
    		p32(0x13BA1000) + 
    		p32(0x14BA1000) + 
    		p32(0x003f0000) +
    		p32(0x0872ffff) +
    		p32(0x30100000) +
    		p32(0x220000b0))