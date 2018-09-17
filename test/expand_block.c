#include <criterion/criterion.h>
#include <string.h>
#include "../malloc.h"

int					expand_block(t_block *b, size_t s);

Test(expand_block, first)
{
	t_page_info	pools[3];

	bzero(pools, sizeof(t_page_info) * 3);
	cr_assert(malloc_init(pools) != 0);

	void *p1 = malloc_internal(16, pools);
	void *p2 = malloc_internal(16, pools);
	void *p3 = malloc_internal(32, pools);

	free_internal(p1, pools);
	free_internal(p2, pools);

	cr_assert(expand_block((t_block *)p1 - 1, 16) == 1);
}
