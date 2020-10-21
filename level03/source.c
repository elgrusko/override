#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

void decrypt(int index) {

    char buf[17] = "Q}|u`sfg~sf{}|a3"; 
    int i;
    
    i = 0;
    while (i < strlen(buf)) {
        buf[i] = buf[i] ^ index;
        i++;
    }
    if (strncmp(buf, "Congratulations!", 17) == 0) {
        system("/bin/sh");
    } else {
        puts("\nInvalid Password");
    }
}

void		test(int pass, int filter) {
	switch (filter - pass) {
		case 1: {
			decrypt(1);
			break;
		}
		case 2: {
			decrypt(2);
			break;
		}
		case 3: {
			decrypt(3);
			break;
		}
		case 4: {
			decrypt(4);
			break;
		}
		case 5: {
			decrypt(5);
			break;
		}
		case 6: {
			decrypt(6);
			break;
		}
		case 7: {
			decrypt(7);
			break;
		}
		case 8: {
			decrypt(8);
			break;
		}
		case 9: {
			decrypt(9);
			break;
		}
		case 16: {
			decrypt(16);
			break;
		}
		case 17: {
			decrypt(17);
			break;
		}
		case 18: {
			decrypt(18);
			break;
		}
		case 19: {
			decrypt(19);
			break;
		}
		case 20: {
			decrypt(20);
			break;
		}
		case 21: {
			decrypt(21);
			break;
		}
		default :
			decrypt(rand());
	}
}

int			main(void) {
	int		pass;

	srand(time(NULL));
	puts("***********************************");
	puts("*\t\tlevel03\t\t**");
	puts("***********************************");

	printf("Password:");
	scanf("%d", &pass);
	test(pass, 0x1337d00d);
	
	return (0);
}
