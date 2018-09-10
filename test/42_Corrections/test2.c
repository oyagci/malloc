#include <stdlib.h>

int main(int argc, char *argv[])
{
	int   i;
	char *addr;

	i = 0;
	while (i < 1024)
	{
		addr = (char *)malloc(1024);
		addr[0] = 42;
		free(addr);
		i += 1;
	}
	return 0;
}
