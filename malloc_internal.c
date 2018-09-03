/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_internal.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/21 14:47:24 by oyagci            #+#    #+#             */
/*   Updated: 2018/09/03 12:48:13 by oyagci           ###   ########.fr       */
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
