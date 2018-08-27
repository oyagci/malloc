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
	add_block_to_free_list(p[5], &pinfo);
	cr_assert(p[5] == pinfo.start->free);
}

Test(add_block_to_free_list, basic2)
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
	add_block_to_free_list(p[5], &pinfo);
	cr_assert(p[5] == pinfo.start->free);

	add_block_to_free_list(p[6], &pinfo);
	cr_assert(p[5] == pinfo.start->free);
	cr_assert(pinfo.start->free->next == p[6]);
}

Test(add_block_to_free_list, basic3)
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
	add_block_to_free_list(p[5], &pinfo);
	cr_assert(p[5] == pinfo.start->free);

	add_block_to_free_list(p[6], &pinfo);
	cr_assert(p[5] == pinfo.start->free);
	cr_assert(pinfo.start->free->next == p[6]);

	add_block_to_free_list(p[7], &pinfo);
	cr_assert(p[5] == pinfo.start->free);
	cr_assert(pinfo.start->free->next == p[6]);
	cr_assert(pinfo.start->free->next->next == p[7]);
}

Test(add_block_to_free_list, basic_reverse)
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
	add_block_to_free_list(p[6], &pinfo);
	cr_assert(p[6] == pinfo.start->free);

	add_block_to_free_list(p[5], &pinfo);
	cr_assert(p[5] == pinfo.start->free);
	cr_assert(pinfo.start->free->next == p[6]);
}
