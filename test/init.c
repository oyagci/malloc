#include <criterion/criterion.h>
#include <sys/mman.h>
#include "../malloc.h"

Test(init_page, basic_tiny)
{
	t_page *p = mmap(
		0,
		M,
		PROT_READ | PROT_WRITE,
		MAP_ANON | MAP_PRIVATE,
		0, 0
	);

	cr_assert(p != 0);
	init_page(p, TINY, M);

	cr_assert(p->next == 0);
	cr_assert(p->prev == 0);
	cr_assert(p->type == TINY);
	cr_assert(p->size == M);
	cr_assert(p->free == 0);
}

Test(init_page_blocks, basic_tiny)
{
	t_page *p = mmap(
		0,
		M,
		PROT_READ | PROT_WRITE,
		MAP_ANON | MAP_PRIVATE,
		0, 0
	);

	cr_assert(p != 0);
	init_page(p, TINY, M);

	init_page_blocks(p);
	
	t_block	*b;

	b = (t_block *)p + 1;

	cr_assert(b->size == p->size - sizeof(t_block) * 2 - sizeof(t_page));
	cr_assert(b->is_free == 1);
	cr_assert(b->prev == 0);
	cr_assert(b->next == (t_block *)((t_byte *)(b + 1) + b->size));
	cr_assert(b->next->size == 0);
}
