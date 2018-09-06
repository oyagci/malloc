#include <criterion/criterion.h>
#include "../malloc.h"
#include <string.h>

Test(malloc_init, basic)
{
	t_page_info	pools[3];

	bzero(pools, sizeof(t_page_info) * 3);

	malloc_init(pools);

	cr_assert(pools[0].start != 0);
	cr_assert(pools[0].start->size == M);

	cr_assert(pools[1].start != 0);
	cr_assert(pools[1].start->size == N);
}
