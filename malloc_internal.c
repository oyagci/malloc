/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_internal.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/21 14:47:24 by oyagci            #+#    #+#             */
/*   Updated: 2018/08/21 15:47:34 by oyagci           ###   ########.fr       */
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

	b = (t_block *)(page + 1);
	b->size = page->size - sizeof(t_block) * 2 - sizeof(t_page);
	b->is_free = 1;
	b->prev = 0;
	b->next = (t_block *)((t_byte *)(b + 1) + b->size);
	b->next->size = 0;
//	b->next->is_free = 0;
//	b->next->prev = b;
//	b->next->next = 0;
}

t_page		*init_new_page()
{
	t_page	*p = map_page(M);

	init_page(p, TINY, M);
	init_page_blocks(p);
	return (p);
}

t_block		*find_free_block(size_t size)
{
	t_page_info		*info;

	info = &g_pools[0];
	if (!info->start)
	{
		info->start = init_new_page();
	}
	(void)size;
	return (0);
}

void		*malloc_internal(size_t size)
{
	t_block	*b;

	b = find_free_block(size);
	return (b ? b + 1 : 0);
}
