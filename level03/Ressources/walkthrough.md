Ici nous avons un binaire nous demandant un mot de passe. Il n'y pas a 
fonction vulnerables aux BoF ni aux format strings. On dirait que nous allons devoir
reverse le binaire : 

Dans la fonction test() nous pouvons voir qu'une variable contenant la valeur 0x1337d00d est presente. 
On voit egalement que notre mot de passe est soustrait a 0x1337d00d. 
Par exemple 0x1337d00d - 1 ===> 0x1333d00c

Nous voyons ensuite qu'un switch est mis en place. Il possede 21 cases qui appelent chacun 
la fonction decrypt(). Le switch utilise le resultat de la soustraction entre notre mot de passe ainsi que la 
valeur 0x1337d00d. 

Un peu plus loin dans le code, nous pouvons voir qu'un XOR est effectue sur chaque char dela string Q}|u`sfg~sf{}|a3
en utilisant le resultat de la fameuse soustraction.

Par exemple : xor 'Q', 15

Ensuite, la le resultat est compare a la string "Congratulations!", auquel cas, un shell pop.

Nous savons que nous devons faire en sorte de trouver quelle valeur XOR doit utiliser pour 
passer de 'Q' de 'Q}|u`sfg~sf{}|a3' a 'C' de 'Congratulations!'

Pour cela, j'ai fait un script python (/scripts/03.py)

	#!/bin/sh

	i = 1
	while i < 0x1337d00d:
    		char = chr(ord("Q") ^ i)
    		if char == "C":
        		print("index is : " + str(i))
        		break
    	i = i + 1

Le script part de 1 pour aller jusqu'a 0x1337d00d (en incrementant i). A chaque tour de boucle
un XOR entre "Q" et i est effectue. Si le resultat est "C" alors nous avons trouver notre index.
Ici, le resultat est 18 ! 

	stri = ""
	for c in "Q}|u`sfg~sf{}|a3":
    		stri += chr(ord(c) ^ 18)
	print(stri)

Notre string est bel et bien transformee en "Congratulations!"
Nous avons donc notre mot de passe ! 

0x1337d00d - 18 = 322424827

Nous entrerons donc dans le 18eme case de notre switch, qui semble etre le bon.



