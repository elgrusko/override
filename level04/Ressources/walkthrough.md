Ce binaire est particulier. 
Tout d'abord un buffer de 32 bytes est declare. Ensuite, le programme fait un `fork`. 
Il va donc se derouler en deux "sous-parties" :

**Le fils : **

Le fils va faire un appel un ptrace() afin qu'il soit trace par le pere et qu'ils puissent "echanger" des signaux.

`ptrace(0, 0, NULL, NULL);`

Ensuite, une string indiquant `Give me some shellcode, k` va etre afficher grace a un puts, puis, via un gets() nous allons 
etre invite a remplir le buffer de 32 bytes : 

        `puts("Give me some shellcode, k");
        gets(str);`
