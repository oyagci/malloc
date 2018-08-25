#include <criterion/criterion.h>
#include "../malloc.h"

Test(free_internal, basic_tiny)
{
	int		*p = malloc_internal(sizeof(int));
	t_block	*b;

	cr_assert(p != NULL);
	free_internal(p);
	b = (t_block *)p - 1;
	cr_assert(b->is_free == 1);
}

Test(add_block_to_free_list, basic)
{
	t_page_info	pinfo;
	void		*p[10];

	pinfo.start = init_new_page(TINY, M);
	cr_assert(pinfo.start);

	for (int i = 0; i < 10; i++)
	{
		p[i] = find_free_block(&pinfo, 16);
		cr_assert(p[i]);
	}
}
