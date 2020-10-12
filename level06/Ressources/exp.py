#!/bin/sh

name = "AAAAAA"
serial_key = (ord(name[3]) ^ 0x1337) + 0x5eeded
i = 0

while i < len(name):
    serial_key = serial_key + (ord(name[i]) ^ serial_key) % 0x539
    i = i + 1
print(serial_key)


