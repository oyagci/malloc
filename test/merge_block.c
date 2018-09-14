#include <criterion/criterion.h>
#include "../malloc.h"
#include <string.h>

Test(merge_free_blocks, free_page)
{
	t_page_info	pools[3];
	int	*array[20];

	bzero(pools, sizeof(t_page_info) * 3);
	for (int i = 0; i < 20; i++)
	{
		array[i] = malloc_internal(sizeof(int) * 10, pools);
		cr_assert(array[i] != 0);
	}

	for (int i = 0; i < 20; i++)
	{
		free_internal(array[i], pools);
	}

	t_block	*b;

	b = (t_block *)(pools[0].start + 1);
	while (b->size != 0)
	{
		b = (t_block *)((t_byte *)(b + 1) + b->size);
	}
}
