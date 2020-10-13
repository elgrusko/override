Ce programme nous demande un username ainsi qu'une serial-key :

    ***********************************
    ************ level06 **************
    ***********************************
    -> Enter Login: AAAAAA
    ***********************************
    ***** NEW ACCOUNT DETECTED ********
    ***********************************
    -> Enter Serial: 123456
 
Ensuite nous pouvons voir qu'un appel a la fonction auth() est effectuee, avec comme arguments, notre username et la serial-key que nous avons entre.
Le retour de la fonction est stocke dans une variable, est si elle equivaut a 0, alors un shell s'ouvre.
(J'ai utilise l'outil Ghidra pour desassembler ce binaire) 

     iVar1 = auth();
     if (iVar1 == 0) {
       puts("Authenticated!");
       system("/bin/sh");
     }

Dans ce fonction auth(), plusieurs choses interessantes se passent :

- Un strnlen est effectue sur notre username, s'assurant que sa longueur est > 5 bytes, sinon la fonction return 1 et le programm exit.

        len_name = strnlen(name,0x20);
        if (len_name < 6) {
          uVar1 = 1;
        }
        [...]
        return uVar1;
        
 Le prochain test effectue, est un appel a ptrace. Ptrace est souvent utilise pour s'assurer que le programme en question n'est pas en train d'etre debuger.
 Pour le bypass avec gdb, il suffit de mettre un breakpoint juste apres son appel, puis de set eax a 0. Cela revient a modifier le retour de ptrace afin de 
 s'assurer qu'il ne considere pas que nous somme en train de debuger.
 
    lVar2 = ptrace(PTRACE_TRACEME);
    if (lVar2 == -1) {
      puts("\x1b[32m.---------------------------.");
      puts("\x1b[31m| !! TAMPERING DETECTED !!  |");
      puts("\x1b[32m\'---------------------------\'");
      uVar1 = 1;
    }
    [...]
    return uVar1;
    
Ensuite, le plus gros du probleme arrive :

      serial_key = ((int)name[3] ^ 0x1337U) + 0x5eeded;
      i = 0;
      while (i < (int)len_name) {
        if (name[i] < ' ') {
          return 1;
        }
        serial_key = serial_key + ((int)name[i] ^ serial_key) % 0x539;
        i = i + 1;
      }
      if (param_2 == serial_key) {
        uVar1 = 0;
      }
      else {
        uVar1 = 1;
      }
      
On peut voir qu'une variable `serial_key` (je l'ai moi-meme rename comme ca, pour que ce soit plus comprehensible) est initialisee
avec la valeur `(int)name[3] ^ 0x1337`, puis que des iterations sont effectuees sur notre username afin de verifier que celui-ci ne comporte pas
de caracteres ASCII < 32 (auquel cas le programme return 1). 
Dans cette meme boucle, notre variable serial_key s'additione avec elle meme + `((int)name[i] ^ serial_key) % 0x539`
Alors fin de la boucle, un test est effectue pour voir si la serial_key que nous avons fourni est la meme que celle qui a ete generee. 

J'ai trouve deux moyens de resoudre de level :

**Avec GDB :**

Tout ce que nous allons avoir a faire est de bypass ptrace et de checker quelles valeurs sont comparees a la fin.

      $ gdb level06
      $ b*auth+114      <- juste apres ptrace
      $ b*auth+286      <- sur le cmp des deux valeurs finales
      $ r
      -> Enter Login: AAAAAA
      -> Enter Serial: 123456
      $ set $eax=0      <- falsifie le retour de ptrace
      $ r
      $ print $eax      (0x1e240 = 123456, soit notre entree)
      $ x $ebp-0x10     (0x005f0c3a = 6229050, soit la valeur attendue en serial-key pour le username 'AAAAAA')
  
  
 Nous avons plus qu'a run le programme avec 'AAAAAA' et '6229050'

       Authenticated !
       
 Bingo!
 
 **Avec Python :**
 
  Voir et lancer le script /Ressources/exp.py
