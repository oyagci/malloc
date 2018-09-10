#include <stdlib.h>

int main()
{
	void *addr;

	int i = 0;
	while (i < 1024)
	{
		addr = malloc(1024);
		i += 1;
	}
	return (0);
}
