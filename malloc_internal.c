/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_internal.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/21 14:47:24 by oyagci            #+#    #+#             */
/*   Updated: 2018/09/03 10:14:03 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include <sys/mman.h>

t_page_info	g_pools[3];

int		round_up(int num, int multiple)
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

void	*map_page(size_t size)
{
	void	*p;

	p = mmap(
		0,
		size,
		PROT_READ | PROT_WRITE,
		MAP_ANON | MAP_PRIVATE,
		0, 0
	);
	return (p);
}

void	init_page(t_page *page, t_page_type type, size_t size)
{
	page->next = 0;
	page->prev = 0;
	page->type = type;
	page->size = size;
	page->free = 0;
}

void	init_page_blocks(t_page *page)
{
	t_block	*b;

	page->free = (t_block *)(page + 1);
	b = page->free;
	b->size = page->size - sizeof(t_block) * 2 - sizeof(t_page);
	b->is_free = 1;
	b->prev = 0;
	b->next = (t_block *)((t_byte *)(b + 1) + b->size);
	b->next->size = 0;
	b->next->is_free = 0;
	b->next->prev = b;
	b->next->next = 0;
}

t_page		*init_new_page(t_page_type type, size_t size)
{
	t_page	*p;
	
	p = map_page(size);
	init_page(p, type, size);
	init_page_blocks(p);
	return (p);
}

t_block		*find_free_block(t_page_info *pinfo, size_t size)
{
	t_block	*b;
	t_block	*c;

	if (!pinfo->start)
	{
		if ((pinfo->start = init_new_page(TINY, M)) == NULL)
			return (0);
	}
	b = pinfo->start->free;
	while (b && !(b->size >= size))
		b = b->next;
	if (b && b->size >= size)
	{
		if (b->size - size > sizeof(t_block) + 16)
		{
			c = (t_block *)((t_byte *)(b + 1) + size);
			c->size = b->size - size - sizeof(t_block) * 2;
			if (b == pinfo->start->free)
				pinfo->start->free = c;
			else
			{
				b->prev->next = c;
				c->prev = b->prev;
			}
			c->next = b->next;
			if (c->next)
				c->next->prev = c;
			b->size = size;
		}
		else
		{
			if (b == pinfo->start->free)
				pinfo->start->free = b->next;
			else
			{
				if (b->prev)
					b->prev->next = b->next;
				if (b->next)
					b->next->prev = b->prev;
			}
		}
		b->prev = NULL;
		b->next = NULL;
		b->is_free = 0;
		return (b);
	}
	return (0);
}

void		*malloc_internal(size_t size, t_page_info *pools)
{
	t_block	*b;

	size = round_up(size, TINY_RES);
	if (size > TINY && size < SMALL)
		size = round_up(size, SMALL_RES);
	b = find_free_block(pools, size);
	return (b ? b + 1 : 0);
}
