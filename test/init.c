#include <criterion/criterion.h>
#include <sys/mman.h>
#include "../malloc.h"

Test(init_page, basic_tiny)
{
	t_page *p = map_page(M);

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
	t_block	*b;

	cr_assert(p != 0);
	init_page(p, TINY, M);

	init_page_blocks(p);

	b = (t_block *)(p + 1);

	cr_assert(p->free == b);
	cr_assert(b->size == p->size - sizeof(t_block) * 2 - sizeof(t_page));
	cr_assert(b->is_free == 1);
	cr_assert(b->prev == 0);
	cr_assert(b->next == (t_block *)((t_byte *)(b + 1) + b->size));
	cr_assert(b->next->size == 0);
}

Test(init_new_page, tiny)
{
	t_page	*p;
	t_block	*b;

	p = init_new_page(TINY, M);

	cr_assert(p != 0);
	init_page(p, TINY, M);
	cr_assert(p->next == 0);
	cr_assert(p->prev == 0);
	cr_assert(p->type == TINY);
	cr_assert(p->size == M);
	cr_assert(p->free == 0);

	init_page_blocks(p);
	b = (t_block *)(p + 1);
	cr_assert(p->free == b);
	cr_assert(b->size == p->size - sizeof(t_block) * 2 - sizeof(t_page));
	cr_assert(b->is_free == 1);
	cr_assert(b->prev == 0);
	cr_assert(b->next == (t_block *)((t_byte *)(b + 1) + b->size));
	cr_assert(b->next->size == 0);
	cr_assert((size_t)b % 16 == 0);
}

Test(find_free_block, basic_tiny)
{
	t_page_info	pool;
	t_page	*p;
	t_block	*b;

	p = init_new_page(TINY, M);
	cr_assert(p != NULL);
	pool.start = p;
	pool.type = TINY;

	b = find_free_block(&pool, 16);
	cr_assert(b != NULL);
	cr_assert(b->size == 16);
	cr_assert(pool.start->free != b);
}
