#include <criterion/criterion.h>
#include "../malloc.h"

Test(malloc, basic_char)
{
	char	*p = malloc_internal(sizeof(char));

	cr_assert(p != 0);
	*p = 'c';
}

Test(malloc, loop_10)
{
	size_t	sizes[10] = { 16, 5, 120,16, 5, 120, 552, 5, 34, 10 };
	void	*p[10];

	for (int i = 0; i < 10; i++)
	{
		p[i] = malloc_internal(sizes[i]);
		*(int *)p[i] = 42;
	}
}

Test(malloc, malloc__free___bigger_malloc)
{
	int	*p;

	p = malloc_internal(16);
	cr_assert(p != 0);
	*p = 42;

	free_internal(p);

	p = malloc_internal(32);
	cr_assert(p != 0);
	*p = 42;
}

Test(malloc, full_page)
{
	t_page_info	pinfo;
	t_block		*b;

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

	void	*p = malloc_internal(15);

	cr_assert(p != 0);
	cr_assert((size_t)p % align == 0);

	p = malloc_internal(15);

	cr_assert(p != 0);
	cr_assert((size_t)p % align == 0);
}

Test(malloc, alignment_small)
{
	size_t align = 16;

	void	*p = malloc_internal(529);

	cr_assert(p != 0);
	cr_assert((size_t)p % align == 0);

	p = malloc_internal(543);

	cr_assert(p != 0);
	cr_assert((size_t)p % align == 0);
}
Test(malloc, block_size_multiple_tiny)
{
	t_block	*b;
	size_t	mult = 16;

	void	*p = malloc_internal(4);
	b = (t_block *)p - 1;

	cr_assert(p != 0);
	cr_assert(b->size % mult == 0);

	p = malloc_internal(1);
	b = (t_block *)p - 1;

	cr_assert(p != 0);
	cr_assert(b->size % mult == 0);
}

Test(malloc, block_size_multiple_small)
{
	t_block	*b;
	size_t	mult = 512;

	void	*p = malloc_internal(1020);
	b = (t_block *)p - 1;

	cr_assert(p != 0);
	cr_assert(b->size % mult == 0);

	p = malloc_internal(1025);
	b = (t_block *)p - 1;

	cr_assert(p != 0);
	cr_assert(b->size % mult == 0);
}
