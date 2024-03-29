/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_internal.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/21 14:47:24 by oyagci            #+#    #+#             */
/*   Updated: 2018/09/19 16:03:11 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

pthread_mutex_t		g_lock = PTHREAD_MUTEX_INITIALIZER;
t_page_info			g_pools[3];

int			round_up(int num, int mult)
{
	return (num = (num + (mult - 1)) & ~(mult - 1));
}

t_block		*find_free_block(t_page_info *pinfo, size_t size)
{
	t_page	*p;
	t_block	*b;
	t_block	*rmder;

	b = 0;
	p = pinfo->start;
	while (p)
	{
		b = p->free;
		while (b && b->size < size)
			b = b->next;
		if (b && b->size >= size)
			break ;
		p = p->next;
	}
	if (!b || b->size == 0)
		return (0);
	if (pinfo->type == TINY && b->size - size > sizeof(t_block) + TINY_RES)
	{
		rmder = (t_block *)((t_byte *)(b + 1) + size);
		rmder->size = b->size - size - sizeof(t_block);
		rmder->is_free = 1;
		b->size = size;
		add_remainder_to_free_list(&p->free, b, rmder);
	}
	else if (pinfo->type == SMALL && b->size - size > sizeof(t_block) + SMALL_RES)
	{
		rmder = (t_block *)((t_byte *)(b + 1) + size);
		rmder->size = b->size - size - sizeof(t_block);
		rmder->is_free = 1;
		b->size = size;
		add_remainder_to_free_list(&p->free, b, rmder);
	}
	else
		remove_block(&p->free, b);
	b->prev = NULL;
	b->next = NULL;
	b->is_free = 0;
	return (b);
}

void		append_page_to_pool(t_page_info *pool)
{
	t_page	*p;

	p = pool->start;
	while (p->next)
		p = p->next;
	p->next = init_new_page(pool->type, pool->start->size);
	p->next->prev = p;
}

int			malloc_init(t_page_info *pools)
{
	if (!pools[0].start)
	{
		pools[0].type = TINY;
		if (NULL == (pools[0].start = init_new_page(TINY, M)))
			return (0);
	}
	if (!pools[1].start)
	{
		pools[1].type = SMALL;
		if (NULL == (pools[1].start = init_new_page(SMALL, N)))
			return (0);
	}
	return (1);
}

void		*malloc_internal(size_t size, t_page_info *pools)
{
	t_block		*b;
	t_page_info	*pool;
	void		*p;

	pthread_mutex_lock(&g_lock);
	if (!malloc_init(pools))
	{
		pthread_mutex_unlock(&g_lock);
		return (0);
	}
	size = get_round_size(size);
	pool = get_pool_for_size(size, pools);
	if (!pool)
	{
		p = malloc_large(pools + 2, size);
		pthread_mutex_unlock(&g_lock);
		return (p);
	}
	b = find_free_block(pool, size);
	if (!b)
	{
		append_page_to_pool(pool);
		b = find_free_block(pool, size);
	}
	pthread_mutex_unlock(&g_lock);
	return (b ? b + 1 : 0);
}
