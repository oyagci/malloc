#include <criterion/criterion.h>
#include "../malloc.h"

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

Test(add_block_to_free_list, prev_value)
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

	t_block	*b;

	b = (t_block *)(pinfo.start + 1);
	while (b->size != 0)
	{
		if (b->next != 0)
			cr_assert(b->prev != b->next);
		b = (t_block *)((t_byte *)(b + 1) + b->size);
	}
}

Test(free_internal, invalid_address)
{
	t_page_info	pools[3];

	bzero(pools, sizeof(t_page_info) * 3);
	malloc_internal(16, pools);
	malloc_internal(16, pools);
	malloc_internal(16, pools);
	malloc_internal(16, pools);
	free_internal((void*)(-1), pools);
}

Test(free_internal, null_value)
{
	t_page_info	pools[3];

	bzero(pools, sizeof(t_page_info) * 3);
	free_internal(0, pools);
}
