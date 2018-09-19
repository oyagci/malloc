#include <criterion/criterion.h>
#include "../malloc.h"

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
	cr_assert(pool.start->free->next != 0);
	cr_assert(pool.start->free->next->size == 0);
	cr_assert(pool.start->free->next->prev == pool.start->free);
}

Test(find_free_block, double_tiny)
{
	t_page_info	pool;
	t_page		*p;
	t_block		*b;

	p = init_new_page(TINY, M);
	cr_assert(p != NULL);
	pool.start = p;
	pool.type = TINY;

	b = find_free_block(&pool, 16);
	cr_assert(b != NULL);
	cr_assert(b->size == 16);
	cr_assert(b->is_free == 0);
	cr_assert(pool.start->free != b);
	cr_assert(pool.start->free->next != 0);
	cr_assert(pool.start->free->next->size == 0);
	cr_assert(pool.start->free->next->prev == pool.start->free);

	t_block	*b2;
	b2 = find_free_block(&pool, 32);
	cr_assert(b2 != b);
	cr_assert(b2->size == 32);
	cr_assert(b2->is_free == 0);
	cr_assert(pool.start->free != b2);
	cr_assert(pool.start->free->next != 0);
	cr_assert(pool.start->free->next->size == 0);
	cr_assert(pool.start->free->next->prev == pool.start->free);
}

Test(find_free_block, unsplittable_block)
{
	t_page_info	pool;

	pool.start = init_new_page(TINY, M);
	pool.type = TINY;

	pool.start->free->size = 16;

	t_block *b = find_free_block(&pool, 16);
	cr_assert(b != 0);

	t_block *b2 = find_free_block(&pool, 16);
	cr_assert(b2 != b);
	cr_assert(b2 == 0);
}

Test(find_free_block, full_page)
{
	t_page_info pool[3];

	bzero(pool, sizeof(t_page_info) * 3);
	pool[0].start = init_new_page(TINY, M);
	cr_assert(pool[0].start != 0);

	pool[0].start->free->is_free = 0;
	pool[0].start->free = pool[0].start->free->next;

	t_block *b = find_free_block(pool, 16);
	cr_assert(b == 0);
}

Test(find_free_block, size_of_remainder)
{
	t_page_info pool[3];

	bzero(pool, sizeof(t_page_info) * 3);
	malloc_init(pool);
	cr_assert(pool[0].start != 0);

	size_t	free_size = pool[0].start->free->size;

	t_block *b = find_free_block(pool, 512);
	cr_assert(b != 0);
	cr_assert(pool[0].start->free->size == free_size - 512 - sizeof(t_block));
}
