uint main(void)

{
  int iVar1;
  char buf[32];
  
  puts("***********************************");
  puts("*\t\tlevel06\t\t*");
  puts("***********************************");
  printf("-> Enter Login: ");
  fgets(local_34,0x20,stdin);
  puts("***********************************");
  puts("***** NEW ACCOUNT DETECTED ********");
  puts("***********************************");
  printf("-> Enter Serial: ");
  __isoc99_scanf();
  iVar1 = auth();
  if (iVar1 == 0) {
    puts("Authenticated!");
    system("/bin/sh");
  }
  }
  return (uint)(iVar1 != 0);
}
