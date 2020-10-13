#include <stdio.h>

int	main(void)
{
	size_t len;
	char pass[100];
	char file_pass[41];
	char user[100];
	FILE *fd;

	memset(pass, '\0', 12);
	memset(file_pass, '\0', 5);
	memset(user, '\0', 12);
	fd = fopen("/home/users/level03/.pass", "r");
	if (fd == NULL)
	{
		fwrite("ERROR: failed to open password file\n", 1, 36, stderr);
		exit(1);
	}
	len = fread(file_pass, 1, 0x29, fd);
	file_pass[strcspn(file_pass, "\n")] = '\0';
	if (len != 41){
		fwrite("ERROR: failed to read password file\n", 1, 36, stderr);
		exit(1);
	}
	fclose(fd);
  	puts("===== [ Secure Access System v1.0 ] =====");
  	puts("***************************************");
  	puts(" You must login to access this system. ");
  	puts("***************************************");
  	printf("--[ Username: ");
	fgets(user, 100, stdin);
	user[strcspn(&user, "\n")] = '\0';
	printf("--[ Password: ");
	fgets(pass, 100, stdin);
	pass[strcspn(&pass, "\n")] = '\0';
	puts("*****************************************");
	if (strncmp(pass, file_pass, 41) == 0) {
		printf("Greetings, %s!\n", user);
		system("/bin/sh");
		return (0);
	}
	printf(user);
	puts(" does not have access!");
	exit(1);
	return (0);
}
