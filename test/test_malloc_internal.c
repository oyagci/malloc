#include <criterion/criterion.h> 
#include "../malloc.h"
#include <string.h>
#include <unistd.h>

Test(malloc_internal, append_page_to_pool)
{
	t_page_info	pools[3];

	bzero(pools, sizeof(t_page_info) * 3);
	pools[0].type = TINY;
	pools[0].start = init_new_page(TINY, M);
	pools[0].start->free->is_free = 0;
	pools[0].start->free = pools[0].start->free->next;

	t_block *b = find_free_block(pools, 16);
	cr_assert(b == 0);

	cr_assert(pools[0].start->next == 0);
	append_page_to_pool(pools);
	cr_assert(pools[0].start->next != 0);
	cr_assert(pools[0].start->prev == 0);
	cr_assert(pools[0].start->next->size == M);
	cr_assert(pools[0].start->next->type == TINY);

	append_page_to_pool(pools);
	cr_assert(pools[0].start->next->next != 0);
	cr_assert(pools[0].start->next->prev != 0);
}

Test(malloc_internal, full_page)
{
	t_page_info	pools[3];

	bzero(pools, sizeof(t_page_info) * 3);

	void *p = malloc_internal(16, pools);
	cr_assert(p != 0);

	t_block *b = (t_block *)p - 1;
	cr_assert(b == (t_block *)(pools[0].start + 1));

	pools[0].start->free->is_free = 0;
	pools[0].start->free = pools[0].start->free->next;
	pools[0].start->free->prev = 0;

	cr_assert(pools[0].start->free->size == 0);
	cr_assert(pools[0].start->free->is_free == 0);

	void *p2 = malloc_internal(16, pools);

	cr_assert(p2 != p);
	cr_assert(p2 != 0);
	cr_assert(((t_block *)p2 - 1)->size != 0);
}

Test(malloc_internal, large)
{
	t_page_info	pools[3];
	t_block		*b;

	bzero(pools, sizeof(t_page_info) * 3);

	void *p = malloc_internal(LARGE * 2, pools);
	cr_assert(p != 0);

	b = (t_block *)p - 1;
	cr_assert(b->size == (size_t)round_up(LARGE * 2, getpagesize()));
}

Test(malloc_internal, size_0)
{
	t_page_info	pools[3];
	t_block		*b;

	bzero(pools, sizeof(t_page_info) * 3);
	malloc_init(pools);

	b = (t_block *)(pools[0].start + 1);
	b->is_free = 0;
	pools[0].start->free = pools[0].start->free->next;

	void *p = malloc_internal(0, pools);
	cr_assert(p != 0);

	b = (t_block *)p - 1;
	cr_assert(b->size == 16);
}

Test(malloc_internal, tiny_max)
{
	t_page_info	pools[3];

	bzero(pools, sizeof(t_page_info) * 3);

	void	*p = malloc_internal(TINY, pools);

	t_block	*first_block = (t_block *)(pools[0].start + 1);

	cr_assert(p != 0);
	cr_assert(pools[0].start->free == (t_block *)((t_byte *)(first_block + 1) + first_block->size));
	cr_assert(first_block->size == TINY);
}

Test(malloc_internal, small_max)
{
	t_page_info	pools[3];

	bzero(pools, sizeof(t_page_info) * 3);

	void	*p = malloc_internal(SMALL, pools);

	t_block	*first_block = (t_block *)(pools[1].start + 1);

	cr_assert(p != 0);
	cr_assert(pools[1].start->free == (t_block *)((t_byte *)(first_block + 1) + first_block->size));
	cr_assert(first_block->size == SMALL);
}
