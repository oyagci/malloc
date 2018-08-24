/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_internal.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/21 14:47:24 by oyagci            #+#    #+#             */
/*   Updated: 2018/08/24 18:30:23 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include <sys/mman.h>

t_page_info	g_pools[3];

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
	if (b->size >= size && b->size > sizeof(t_block) + 16)
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
		b->size = size;
		b->prev = NULL;
		b->next = NULL;
		return (b);
	}
	return (0);
}

void		*malloc_internal(size_t size)
{
	t_block	*b;

	b = find_free_block(&g_pools[0], size);
	return (b ? b + 1 : 0);
}
