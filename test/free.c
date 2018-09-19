#include <criterion/criterion.h>
#include "../malloc.h"
#include <string.h>

Test(add_block_to_free_list, basic)
{
	t_page_info	pinfo[3];
	void		*p[10];

	bzero(pinfo, sizeof(t_page_info) * 3);
	malloc_init(pinfo);
	cr_assert(pinfo->start);

	for (int i = 0; i < 10; i++)
	{
		p[i] = find_free_block(pinfo, 16);
		cr_assert(p[i] != NULL);
	}
	add_block_to_free_list(p[5], pinfo);
	cr_assert(p[5] == pinfo->start->free);
}

Test(add_block_to_free_list, basic2)
{
	t_page_info	pinfo[3];
	void		*p[10];

	bzero(pinfo, sizeof(t_page_info) * 3);
	malloc_init(pinfo);
	cr_assert(pinfo->start);

	for (int i = 0; i < 10; i++)
	{
		p[i] = find_free_block(pinfo, 16);
		cr_assert(p[i]);
	}
	add_block_to_free_list(p[5], pinfo);
	cr_assert(p[5] == pinfo->start->free);

	add_block_to_free_list(p[6], pinfo);
	cr_assert(p[5] == pinfo->start->free);
	cr_assert(pinfo->start->free->next == p[6]);
}

Test(add_block_to_free_list, basic3)
{
	t_page_info	pinfo[3];
	void		*p[10];

	bzero(pinfo, sizeof(t_page_info) * 3);
	malloc_init(pinfo);
	cr_assert(pinfo->start);

	for (int i = 0; i < 10; i++)
	{
		p[i] = find_free_block(pinfo, 16);
		cr_assert(p[i]);
	}
	add_block_to_free_list(p[5], pinfo);
	cr_assert(p[5] == pinfo->start->free);

	add_block_to_free_list(p[6], pinfo);
	cr_assert(p[5] == pinfo->start->free);
	cr_assert(pinfo->start->free->next == p[6]);

	add_block_to_free_list(p[7], pinfo);
	cr_assert(p[5] == pinfo->start->free);
	cr_assert(pinfo->start->free->next == p[6]);
	cr_assert(pinfo->start->free->next->next == p[7]);
}

Test(add_block_to_free_list, basic_reverse)
{
	t_page_info	pinfo[3];
	void		*p[10];

	bzero(pinfo, sizeof(t_page_info) * 3);
	malloc_init(pinfo);
	cr_assert(pinfo->start);

	for (int i = 0; i < 10; i++)
	{
		p[i] = find_free_block(pinfo, 16);
		cr_assert(p[i] != 0);
	}
	add_block_to_free_list(p[6], pinfo);
	cr_assert(p[6] == pinfo->start->free);

	add_block_to_free_list(p[5], pinfo);
	cr_assert(p[5] == pinfo->start->free);
	cr_assert(pinfo->start->free->next == p[6]);
}

Test(add_block_to_free_list, prev_value)
{
	t_page_info	pinfo[3];
	void		*p[10];

	bzero(pinfo, sizeof(t_page_info) * 3);
	malloc_init(pinfo);
	cr_assert(pinfo->start);

	for (int i = 0; i < 10; i++)
	{
		p[i] = find_free_block(pinfo, 16);
		cr_assert(p[i]);
	}
	add_block_to_free_list(p[6], pinfo);
	cr_assert(p[6] == pinfo->start->free);

	add_block_to_free_list(p[5], pinfo);
	cr_assert(p[5] == pinfo->start->free);
	cr_assert(pinfo->start->free->next == p[6]);

	t_block	*b;

	b = (t_block *)(pinfo->start + 1);
	while (b->size != 0)
	{
		if (b->next != 0 && b->next != (void*)0xdeadbeef)
			cr_expect(b->prev != b->next);
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

Test(free_internal, free_list_state__simple)
{
	t_page_info	pools[3];
	t_block		*b;

	bzero(pools, sizeof(t_page_info) * 3);
	cr_assert(malloc_init(pools) != 0);

	void *p1 = malloc_internal(16, pools);

	add_block_to_free_list((t_block *)p1 - 1, pools);

	t_block *f = pools[0].start->free;

	cr_assert(pools[0].start->next == 0);
	cr_assert(f == (t_block *)p1 - 1);
}

Test(free_internal, free_list_state)
{
	t_page_info	pools[3];

	bzero(pools, sizeof(t_page_info) * 3);
	cr_assert(malloc_init(pools) != 0);

	void *p1 = malloc_internal(16, pools);
	void *p2 = malloc_internal(16, pools);
	void *p3 = malloc_internal(32, pools);

	add_block_to_free_list((t_block *)p1 - 1, pools);
	add_block_to_free_list((t_block *)p2 - 1, pools);

	t_block *f = pools[0].start->free;

	cr_assert(pools[0].start->next == 0);
	cr_assert(f == (t_block *)p1 - 1);
	cr_assert(f->next == (t_block *)p2 - 1);
}
