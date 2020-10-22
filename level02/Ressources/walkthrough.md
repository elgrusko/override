En executant le binaire, on peut voir qu'il nous demande un username ainsi
qu'un password. 
Le fichier "/home/users/level03/.pass" est egalement lu avec fread puis stocke sur la stack.
Si nous entrons le bon mot de passe, un shell pop. 

    
    Guessed arguments:
    arg[0]: 0x7fffffffdf40 ('A' <se r\377\377\377\377\377\377\377\377p\377\377\377\377\377\377\377\377te 41 fois>)         <--- contenu de .pass
    arg[1]: 0x7fffffffded0 --> 0x61746174 ('tata')
    arg[2]: 0x29 (')')
    arg[3]: 0x7fffffffded0 --> 0x61746174 ('tata')


Apres plus plusieurs tentatives de BoF, il semblerait que cela ne soit pas possible.

Cependant quelque chose d'interessant saute au yeux : Si nous entrons 
les mauvais identifiants, un printf affiche le username que nous 
avons passe au programme. Ensuite, exit() est appele. 

printf semble ne prendre aucun autre argument que notre entree ! Essayons d'exploiter 
une format string! 

En rentrant '%x' en tant que username on peut voir qu'a la fin de l'execution du programme, une adresse de la stack apparait. 
Nous savons que le strncmp permettant de comparer notre password avec le fichier .pass (sur 41 bytes). 
Nous savons egalement que le contenu du fichier .pass est stocke sur la stack.

Pourquoi ne pas simplement dumper la stack a l'aide du formateur %x puis essayer de reperer ce qui s'apparente a une string de 41 bytes...? 
Le programme etant petit, il doit pas y avoir grand chose en memoire.

En copiant le binaire via scp et en creant nous meme un fichier "/home/users/level03/.pass" dans lequel nous inserons 40 'A' et que nous dumpons
la stack (voir /ressources/exploit) on peut voir que nos 0x414141414141 s'affichent a partir de %22\$p.


Nous savons donc que c'est a partir d'ici que le contenu du vrai fichier .pass sera stocke.

Il suffit de lancer notre script (Ressources/exploit) sur le vrai binaire, puis de conserver les valeurs a partir de %22\$p.

		*copier le contenu du script dans /tmp/exploit*
		$ cd /tmp
		$ chmod +x exploit
        	$ ./exploit

        0x756e505234376848 does not have access!
        0x45414a3561733951 does not have access!
        0x377a7143574e6758 does not have access!
        0x354a35686e475873 does not have access!
        0x48336750664b394d does not have access!

En convertissant bout a bout chaque suite de caracteres en ASCII nous obtenons :

        Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H
