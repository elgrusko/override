int auth(char *name, uint key)

{
  size_t len_name;
  int uVar1;
  long lVar2;
  int i;
  uint serial_key;
  
  len_name = strcspn(name,"\n");
  name[len_name] = '\0';
  len_name = strnlen(name,0x20);
  if ((int)len_name < 6) {
    uVar1 = 1;
  }
  else {
    lVar2 = ptrace(PTRACE_TRACEME);
    if (lVar2 == -1) {
      puts("\x1b[32m.---------------------------.");
      puts("\x1b[31m| !! TAMPERING DETECTED !!  |");
      puts("\x1b[32m\'---------------------------\'");
      uVar1 = 1;
    }
    else {
      serial_key = ((int)name[3] ^ 0x1337U) + 0x5eeded;
      i = 0;
      while (i < (int)len_name) {
        if (name[i] < ' ') {
          return 1;
        }
        serial_key = serial_key + ((int)name[i] ^ serial_key) % 0x539;
        i = i + 1;
      }
      if (key == serial_key) {
        uVar1 = 0;
      }
      else {
        uVar1 = 1;
      }
    }
  }
  return uVar1;
}

int main(void)
{
  int iVar1;
  char buf[32];
  
  puts("***********************************");
  puts("*\t\tlevel06\t\t*");
  puts("***********************************");
  printf("-> Enter Login: ");
  fgets(buf, 0x20, stdin);
  puts("***********************************");
  puts("***** NEW ACCOUNT DETECTED ********");
  puts("***********************************");
  printf("-> Enter Serial: ");
  scanf('%s', &buf);
  iVar1 = auth();
  if (iVar1 == 0) {
      puts("Authenticated!");
      system("/bin/sh");
  }
  return 0;
}
