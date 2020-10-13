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

     iVar1 = auth();
     if (iVar1 == 0) {
       puts("Authenticated!");
       system("/bin/sh");
     }
