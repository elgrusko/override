#include <stdio.h>
#include <stdlib.h>

int	main(void)
{

	char *ptr = getenv("SHELLCODE");
	printf("%p\n", ptr);
	return (0);
}

