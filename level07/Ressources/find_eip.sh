#!/bin/sh

i=100
while [ $i -lt 200 ]; do
	python -c "print('read\n' + '$i' + '\n' + 'quit\n')"  > /tmp/payload
 	cat /tmp/payload | /home/users/level07/level07 | grep "4158936339"
	i=$(( i+1 ))
done
