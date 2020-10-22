#!/bin/sh

i = 1
while i < 0x1337d00d:
   		char = chr(ord("Q") ^ i)
   		if char == "C":
       		print("index is : " + str(i))
       		break
   	i = i + 1
