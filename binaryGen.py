from pwn import *

with open('bin','w') as f:
    f.write(p32(0x000f0004) + 
    		p32(0x13312000) + 
    		p32(0x14412000) + 
    		p32(0x005f0000) +
    		p32(0x0872ffff) +
    		p32(0x30100000) +
    		p32(0x220000b0))