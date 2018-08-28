#include <criterion/criterion.h>
#include "malloc.h"

Test(malloc, basic_char)
{
	char	*p = malloc_internal(sizeof(char));

	cr_assert(p != 0);
	*p = 'c';
}

Test(malloc, loop_10)
{
	size_t	sizes[10] = { 16, 5, 120,16, 5, 120, 552, 5, 34, 10 };
	void	*p[10];

	for (int i = 0; i < 10; i++)
	{
		p[i] = malloc_internal(sizes[i]);
		*(int *)p[i] = 42;
	}
}

Test(malloc, malloc__free___bigger_malloc)
{
	int	*p;

	p = malloc_internal(16);
	cr_assert(p != 0);
	*p = 42;

	free_internal(p);

	p = malloc_internal(32);
	cr_assert(p != 0);
	*p = 42;
}
