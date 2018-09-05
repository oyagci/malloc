/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_internal.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/21 14:47:24 by oyagci            #+#    #+#             */
/*   Updated: 2018/09/05 10:54:40 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include <sys/mman.h>

t_page_info	g_pools[3];

int			round_up(int num, int multiple)
{
	int	remainder;

	if (num == 0)
		return (multiple);
	if (multiple == 0)
		return (num);
	remainder = num % multiple;
	if (remainder == 0)
		return (num);
	return (num + multiple - remainder);
}

void		*map_page(size_t size)
{
	void	*p;

	p = mmap(
		0,
		size,
		PROT_READ | PROT_WRITE,
		MAP_ANON | MAP_PRIVATE,
		0, 0);
	return (p);
}

t_block		*find_free_block(t_page_info *pinfo, size_t size)
{
	t_page	*p;
	t_block	*b;
	t_block	*rmder;

	/* TODO: Initialisation should not happen here */
	if (!pinfo->start)
		if (NULL == (pinfo->start = init_new_page(TINY, M)))
			return (0);
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
	if (b->size - size > sizeof(t_block) + 16)
	{
		rmder = (t_block *)((t_byte *)(b + 1) + size);
		rmder->size = b->size - size - sizeof(t_block);
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

void		*malloc_internal(size_t size, t_page_info *pools)
{
	t_block	*b;

	size = round_up(size, TINY_RES);
	if (size > TINY && size < SMALL)
		size = round_up(size, SMALL_RES);
	b = find_free_block(pools, size);
	if (!b)
	{
		append_page_to_pool(pools);
		b = find_free_block(pools, size);
	}
	return (b ? b + 1 : 0);
}
