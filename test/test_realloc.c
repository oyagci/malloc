#include <criterion/criterion.h>
#include <string.h>
#include "../malloc.h"

Test(realloc_internal, basic)
{
	t_page_info	pools[3];

	bzero(pools, sizeof(t_page_info) * 3);

	void *p1 = malloc_internal(16, pools);
	cr_assert(p1 != 0);

	free_internal(p1, pools);

	void *p2 = malloc_internal(16, pools);
	cr_assert(p2 == p1);
}

Test(realloc_internal, from_tiny_to_small)
{
	t_page_info pools[3];

	bzero(pools, sizeof(t_page_info) * 3);

	void *p1 = malloc_internal(TINY_RES * 3, pools);
	pools[0].start->free = pools[0].start->free->next;
	pools[0].start->free->prev = 0;

	memset(p1, 42, TINY_RES * 3);

	void *p2 = realloc_internal(p1, SMALL_RES * 3, pools);

	memset(p2, 42, SMALL_RES * 3);
}

Test(realloc_internal, from_tiny_to_large)
{
	t_page_info pools[3];

	bzero(pools, sizeof(t_page_info) * 3);

	void *p1 = malloc_internal(TINY_RES * 3, pools);
	pools[0].start->free = pools[0].start->free->next;
	pools[0].start->free->prev = 0;

	memset(p1, 42, TINY_RES * 3);

	void *p2 = realloc_internal(p1, SMALL + 1, pools);

	memset(p2, 42, SMALL + 1);
}

Test(realloc_internal, from_small_to_large)
{
	t_page_info pools[3];

	bzero(pools, sizeof(t_page_info) * 3);

	void *p1 = malloc_internal(SMALL_RES * 3, pools);

	memset(p1, 42, SMALL_RES * 2);

	void *p2 = realloc_internal(p1, SMALL + 1, pools);

	memset(p2, 42, SMALL + 1);
}
