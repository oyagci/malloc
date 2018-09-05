#include <criterion/criterion.h>
#include "../malloc.h"
#include <string.h>

Test(malloc_large, basic)
{
	t_page_info	pools[3];

	bzero(pools,sizeof(t_page_info) * 3);
	void *p = malloc_large(pools + 2, LARGE);
	cr_assert(p != 0);
}

Test(malloc_large, two_pages)
{
	t_page_info	pools[3];

	bzero(pools,sizeof(t_page_info) * 3);
	void *p = malloc_large(pools + 2, LARGE);
	cr_assert(p != 0);

	void *p2 = malloc_large(pools + 2, LARGE);
	cr_assert(p2 != 0);
	cr_assert(p2 != p);
}

Test(malloc_large, three_pages)
{
	t_page_info	pools[3];

	bzero(pools,sizeof(t_page_info) * 3);
	void *p = malloc_large(pools + 2, LARGE);
	cr_assert(p != 0);

	void *p2 = malloc_large(pools + 2, LARGE);
	cr_assert(p2 != 0);
	cr_assert(p2 != p);

	void *p3 = malloc_large(pools + 2, LARGE);
	cr_assert(p3 != 0);
	cr_assert(p3 != p);
	cr_assert(p3 != p2);
}
