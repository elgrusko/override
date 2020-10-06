En desassemblant la fonction main :

	$ gdb level00
	$ disas main

On peut voir qu'un cmp est realise jsute apres la fonction scanf().
Il verifie que notre string est egale a 0x149c (5276 en decimal), puis, ouvre
un shell si c'est True.

   0x080484de <+74>:	call   0x80483d0 <__isoc99_scanf@plt>
   0x080484e3 <+79>:	mov    eax,DWORD PTR [esp+0x1c]
   0x080484e7 <+83>:	cmp    eax,0x149c


	$ ./level00 5276
	$ whoami
	=> level01
