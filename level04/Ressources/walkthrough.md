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

Nous avons juste besoin de l'offset necessaire pour ecraser EIP, puis de l'adresse de notre shellcode.
Apres plusieurs tentatives de `print('A'*x)` nous pouvons en deduire que l'offset est de 156.

