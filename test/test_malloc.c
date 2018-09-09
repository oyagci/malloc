#include <criterion/criterion.h>
#include "../malloc.h"
#include <string.h>

Test(malloc, basic_char)
{
	t_page_info	pools[3];

	bzero(pools, sizeof(t_page_info) * 3);

	char	*p = malloc_internal(sizeof(char), pools);

	cr_assert(p != 0);
	*p = 'c';
}

Test(malloc, loop_10)
{
	t_page_info	pools[3];
	size_t	sizes[10] = { 16, 5, 120,16, 5, 120, 552, 5, 34, 10 };
	void	*p[10];

	bzero(pools, sizeof(t_page_info) * 3);
	for (int i = 0; i < 10; i++)
	{
		p[i] = malloc_internal(sizes[i], pools);
		*(int *)p[i] = 42;
	}
}

Test(malloc, malloc__free___bigger_malloc)
{
	int	*p;
	t_page_info	pools[3];

	bzero(pools, sizeof(t_page_info) * 3);
	p = malloc_internal(16, pools);
	cr_assert(p != 0);
	*p = 42;

	free_internal(p, pools);

	p = malloc_internal(32, pools);
	cr_assert(p != 0);
	*p = 42;
}

Test(malloc, full_page)
{
	t_page_info	pools[3];
	t_page_info	pinfo;
	t_block		*b;

	bzero(pools, sizeof(t_page_info) * 3);
	pinfo.start = init_new_page(TINY, M);
	pinfo.type = TINY;
	b = pinfo.start->free;
	pinfo.start->free = b->next;
	b->prev = 0;
	b->next = 0;
	b->is_free = 0;

	t_block *f = find_free_block(&pinfo, 16);
	cr_assert(f == NULL);
}

Test(malloc, alignment_tiny)
{
	size_t align = 16;
	t_page_info	pools[3];

	bzero(pools, sizeof(t_page_info) * 3);
	void	*p = malloc_internal(15, pools);

	cr_assert(p != 0);
	cr_assert((size_t)p % align == 0);

	p = malloc_internal(15, pools);

	cr_assert(p != 0);
	cr_assert((size_t)p % align == 0);
}

Test(malloc, alignment_small)
{
	size_t align = 16;
	t_page_info	pools[3];

	bzero(pools, sizeof(t_page_info) * 3);
	void	*p = malloc_internal(529, pools);

	cr_assert(p != 0);
	cr_assert((size_t)p % align == 0);

	p = malloc_internal(543, pools);

	cr_assert(p != 0);
	cr_assert((size_t)p % align == 0);
}
Test(malloc, block_size_multiple_tiny)
{
	t_block	*b;
	size_t	mult = 16;
	t_page_info	pools[3];

	bzero(pools, sizeof(t_page_info) * 3);

	void	*p = malloc_internal(4, pools);
	b = (t_block *)p - 1;

	cr_assert(p != 0);
	cr_assert(b->size % mult == 0);

	p = malloc_internal(1, pools);
	b = (t_block *)p - 1;

	cr_assert(p != 0);
	cr_assert(b->size % mult == 0);
}

Test(malloc, block_size_multiple_small)
{
	t_block	*b;
	size_t	mult = 512;
	t_page_info	pools[3];

	bzero(pools, sizeof(t_page_info) * 3);

	void	*p = malloc_internal(1020, pools);
	b = (t_block *)p - 1;

	cr_assert(p != 0);
	cr_assert(b->size % mult == 0);

	p = malloc_internal(1025, pools);
	b = (t_block *)p - 1;

	cr_assert(p != 0);
	cr_assert(b->size % mult == 0);
}
