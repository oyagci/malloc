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
