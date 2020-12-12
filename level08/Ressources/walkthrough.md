En analysant le binaire, on voit qu'il ne fait pas enormement de choses :

Il prend un argument : un fichier
Il ouvre le fichier "./backups/.log" puis ecrit dedans "Starting back up: <nom_fichier>" et garde
un fd ouvert sur ce fichier.
Ensuite, il ouvre le fichier que nous lui avons passe en parametre, le probleme, c'est qu'il
utile un path relatif et non absolu : ./backups/

Du coup, il nous suffit de recreer ce chemin dans un dossier dans lequel nous avons les droits :

	$ cd /tmp
	$ mkdir -p backups/home/users/level09

Maintenant il nous suffit d'appeler le binaire via /tmp :

	$ ~/level08 /home/users/level09/.pass

Le contenu de /home/users/level09/.pass va etre ecrit dans /tmp/backups/home/users/level09/.pass !

	$ cat /tmp/backups/home/users/level09/.pass

	fjAwpJNs2vvkFLRebEvAQ2hFZ4uQBWfHRsP62d8S
