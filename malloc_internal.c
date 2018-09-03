/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_internal.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/21 14:47:24 by oyagci            #+#    #+#             */
/*   Updated: 2018/09/03 12:41:11 by oyagci           ###   ########.fr       */
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

void		remove_block(t_block **free_list, t_block *to_remove)
{
	if (to_remove == *free_list)
		*free_list = to_remove->next;
	else
	{
		if (to_remove->prev)
			to_remove->prev->next = to_remove->next;
		if (to_remove->next)
			to_remove->next->prev = to_remove->prev;
	}
}

void		add_remainder_to_free_list(
		t_block **free_list,
		t_block *b,
		t_block *rmder
)
{
	if (b == *free_list)
		*free_list = rmder;
	else
	{
		b->prev->next = rmder;
		rmder->prev = b->prev;
	}
	rmder->next = b->next;
	if (rmder->next)
		rmder->next->prev = rmder;
}

t_block		*find_free_block(t_page_info *pinfo, size_t size)
{
	t_block	*b;
	t_block	*rmder;

	if (!pinfo->start)
		if (NULL == (pinfo->start = init_new_page(TINY, M)))
			return (0);
	b = pinfo->start->free;
	while (b && b->size < size)
		b = b->next;
	if (!b)
		return (0);
	if (b->size - size > sizeof(t_block) + 16)
	{
		rmder = (t_block *)((t_byte *)(b + 1) + size);
		rmder->size = b->size - size - sizeof(t_block) * 2;
		b->size = size;
		add_remainder_to_free_list(&pinfo->start->free, b, rmder);
	}
	else
		remove_block(&pinfo->start->free, b);
	b->prev = NULL;
	b->next = NULL;
	b->is_free = 0;
	return (b);
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
